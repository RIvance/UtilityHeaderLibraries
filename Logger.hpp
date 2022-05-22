
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>
#include <chrono>
#include <iomanip>

namespace logger
{
    namespace format
    {
        static inline void printErrLn() { std::cerr << '\n'; }

        template<typename T, typename ... Ts>
        static inline void printErrLn(const T & first, Ts ... tail)
        {
            std::cerr << first << ' ';
            printErrLn(tail...);
        }

        #define COLOR_DEF(color, c0, c1)  static const char* color = "\033[" #c0 ";" #c1 "m"

            COLOR_DEF(RED,      0, 31);
            COLOR_DEF(GREEN,    0, 32);
            COLOR_DEF(YELLOW,   0, 33);
            COLOR_DEF(BLUE,     0, 34);
            COLOR_DEF(CYAN,     0, 36);

        #undef COLOR_DEF

        static inline std::string colorString(const std::string & s, const char* color)
        {
            return color + s + "\033[0m";
        }

        static inline std::string timeString()
        {
            time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            tm timeinfo = *std::localtime(&now);
            char buffer[64];
            std::strftime(buffer, sizeof(buffer),"%Y-%m-%d %H:%M:%S", &timeinfo);
            return buffer;
        }
    }

    template<typename ... Ts>
    static inline void error(Ts ... args)
    {
        std::cerr << format::colorString("[", format::BLUE);
        std::cerr << format::colorString(" ERROR ", format::RED);
        std::cerr << format::timeString() << " ";
        std::cerr << format::colorString("]", format::BLUE) << " ";
        format::printErrLn(args...);
    }

    template<typename ... Ts>
    static inline void warning(Ts ... args)
    {
        std::cerr << format::colorString("[", format::BLUE);
        std::cerr << format::colorString(" WARN  ", format::YELLOW);
        std::cerr << format::timeString() << " ";
        std::cerr << format::colorString("]", format::BLUE) << " ";
        format::printErrLn(args...);
    }

    template<typename ... Ts>
    static inline void debug(Ts ... args)
    {
        std::cerr << format::colorString("[", format::BLUE);
        std::cerr << format::colorString(" DEBUG ", format::CYAN);
        std::cerr << format::timeString() << " ";
        std::cerr << format::colorString("]", format::BLUE) << " ";
        format::printErrLn(args...);
    }

    template<typename ... Ts>
    static inline void info(Ts ... args)
    {
        std::cerr << format::colorString("[", format::BLUE);
        std::cerr << format::colorString(" INFO  ", format::GREEN);
        std::cerr << format::timeString() << " ";
        std::cerr << format::colorString("]", format::BLUE) << " ";
        format::printErrLn(args...);
    }
}

#endif // LOGGER_HPP
