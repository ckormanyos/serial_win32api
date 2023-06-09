serial_win32api
==================

serial_win32api implements a modernized, C++14, header-only serial (COM) driver
for use with the classic Win32-API.

Initially written in a very old dialect of C++ in 1998, this work has been modernized
for use with C++14 and beyond.

## Usage

Using serial_win32api is straightforward.
  - Include the header `serial_win32api.h`.
  - Ensure that its path is included in the compiler's include paths.
  - Use the constructor, and the `send()` and `recv()` functions.

```cpp
#include <vector>

#include <serial_win32api.h>

auto main() -> int
{
  // Open COM3.
  serial_win32api ser { static_cast<std::uint32_t>(UINT8_C(3)) };

  // Check validity of COM3.
  if(ser.valid())
  {
    // Send three bytes over COM3.

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
