
# Properties.hpp

A library to automatically initialize varibles at *runtime*.

`test.prop`

```
intValue = 1
doubleValue = 3.14
str = abcdef
```

`main.cpp`

```cpp
#include "Properties.hpp"

#include <iostream>

PROPERTY_CONFIG("test.prop");

int $(intValue);
double $(doubleValue);
string $(str);

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

