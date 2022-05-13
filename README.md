
# Properties.hpp

A library to automatically initialize varibles at *runtime*.

`test.prop`

```
count = 1
vel = 3.14
name = abcdef
```

`main.cpp`

```cpp
#include "Properties.hpp"

#include <iostream>

$PropertyFile("test.prop");

int $Property(intValue);
double $Property(doubleValue);
string $Property(str);

int main()
{
    std::cout << intValue << " " << doubleValue << " " << str;
    std::cout << std::endl;
    return 0;
}
```

---

# TerminalImshow.hpp

A library to show image in the terminal

## Usage

```cpp
#include "TerminalImshow.hpp"

int main(int argc, char* argv[])
{
    cv::imread("test.png") >> stdout;
}
```

## CMake

```cmake
find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRECTORS})
target_link_libraries(${BIN_NAME} pthread ${OpenCV_LIBS})
```

## References

- [ANSI Escape Code](https://en.wikipedia.org/wiki/ANSI_escape_code)

- [Block Elements](https://en.wiktionary.org/wiki/Appendix:Unicode/Block_Elements)

