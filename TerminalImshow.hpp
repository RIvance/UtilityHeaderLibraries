#ifndef TERM_IMSHOW_HPP
#define TERM_IMSHOW_HPP

#include <opencv2/opencv.hpp>

#include <cstdint>
#include <string>
#include <thread>

#include <sys/ioctl.h>
#include <unistd.h>

using String = std::string;

class Color
{
  public:

    uint8_t r = 0, g = 0, b = 0;

    Color() = default;
    inline Color(uint8_t r, uint8_t g, uint8_t b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    inline uint8_t & operator[] (int index)
    {
        switch (index) {
            case 0: return r;
            case 1: return g;
            case 2: return b;
            default: throw std::exception();
        }
    }

    inline bool operator == (const Color & rhs) const
    {
        return r == rhs.r && g == rhs.g && b == rhs.b;
    }
};

namespace TerminalImshow
{
    #define PIXEL "\u2580"

    class TermPixel
    {
      private:

        Color upper, lower;

      public:

        TermPixel() = default;

        TermPixel(const Color & upper, const Color & lower)
          : upper(upper), lower(lower)
        { /* empty */ }

        inline String toString() const
        {
            return "\033[38;2;" +
                std::to_string(upper.b) +
            ";" +
                std::to_string(upper.g) +
            ";" +
                std::to_string(upper.r) +
            "m" +
            "\033[48;2;" +
                std::to_string(lower.b) +
            ";" +
                std::to_string(lower.g) +
            ";" +
                std::to_string(lower.r) +
            "m" + PIXEL;
        }

        inline bool operator == (const TermPixel & rhs) const
        {
            return this->upper == rhs.upper && this->lower == rhs.lower;
        }

    };

    inline cv::Size getTerminalSize()
    {
        int cols, rows;

      #ifdef TIOCGSIZE

        ttysize ts { };
        ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
        cols = ts.ts_cols;
        rows = ts.ts_lines;

      #elif defined(TIOCGWINSZ)

        winsize ts{};
        ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
        cols = ts.ws_col;
        rows = ts.ws_row;

      #endif /* TIOCGSIZE */

        return { cols, rows - 1 };
    }

    void render(const cv::Mat & src, FILE* out)
    {
        std::thread([&src, out]() -> void {

            cv::Mat image;
            cv::Size size = getTerminalSize();
            size.height *= 2;

            cv::resize(src, image, size);

            // hide cursor
            fputs("\033[?25l\033[0;0H", out);

            String buffer;
            buffer.resize(size.width * size.height * sizeof(PIXEL) * 2);

            for (int row = 0; row < size.height; row += 2) {

                TermPixel lastPixel;

                for (int col = 0; col < size.width; col++) {
                    Color upperColor = image.at<Color>(row,     col);
                    Color lowerColor = image.at<Color>(row + 1, col);
                    TermPixel pixel(upperColor, lowerColor);

                    if (col > 0 && pixel == lastPixel) {
                        buffer.append(PIXEL);
                    } else {
                        buffer.append(pixel.toString());
                        lastPixel = pixel;
                    }
                }

                buffer.append("\n");
            }

            fwrite(buffer.c_str(), buffer.size(), 1, out);

            // show cursor
            fputs("\033[?25h", out);

        }).join();
    }

    #undef PIXEL
}

void operator >> (const cv::Mat & src, FILE* out)
{
    TerminalImshow::render(src, out);
}

#endif // TERM_IMSHOW_HPP
