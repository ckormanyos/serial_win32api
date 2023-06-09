serial_win32api
==================

<p align="center">
    <a href="https://github.com/ckormanyos/serial_win32api/actions">
        <img src="https://github.com/ckormanyos/serial_win32api/actions/workflows/serial_win32api.yml/badge.svg" alt="Build Status"></a>
</p>

serial_win32api implements a modern, C++14, header-only serial (COM) driver.
It is intended to be used with MSVC and the classic Win32-API.

## Usage

Using serial_win32api is straightforward.
  - Include the header `<serial_win32api.h>`.
  - Ensure that its path is included in the compiler's include paths.
  - Use the constructor, and the `send()` and `recv()` functions.

## Example

```cpp
#include <serial_win32api.h>

auto main() -> int
{
  // Open COM3.
  serial_win32api ser { static_cast<std::uint32_t>(UINT8_C(3)) };

  // Check the validity of COM3.
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

## History

This work has been modernized in 2023 for use with C++14 and beyond.
It was originally written in a very old dialect of C++ in 1998.
