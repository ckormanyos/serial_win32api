serial_win32api
==================

serial_win32api implements a C++14 serial (COM) driver for use with the classic Win32-API.

This project was initially written in a very old dialect of C++ in 1998 and has been modernized
for use with C++14 and beyond.

It features a modern C++ header-only design.

```cpp
#include <vector>

#include <serial_win32api.h>

auto main() -> int
{
  // Open COM3
  serial_win32api ser { static_cast<std::uint32_t>(UINT8_C(3)) };

  // Check validity.
  if(ser.valid())
  {
    // Send three bytes.

    const auto bytes_to_send =
      std::vector<std::uint8_t>
      (
        {
          static_cast<std::uint8_t>('a'),
          static_cast<std::uint8_t>('b'),
          static_cast<std::uint8_t>('c')
        }
      );

    static_cast<void>(ser.send(bytes_to_send));
  }
}
```
