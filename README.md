serial_win32api
==================

<p align="center">
    <a href="https://github.com/ckormanyos/serial_win32api/actions">
        <img src="https://github.com/ckormanyos/serial_win32api/actions/workflows/serial_win32api.yml/badge.svg" alt="Build Status"></a>
    <a href="https://github.com/ckormanyos/serial_win32api/issues?q=is%3Aissue+is%3Aopen+sort%3Aupdated-desc">
        <img src="https://custom-icon-badges.herokuapp.com/github/issues-raw/ckormanyos/serial_win32api?logo=github" alt="Issues" /></a>
    <a href="https://github.com/ckormanyos/serial_win32api/blob/master/LICENSE_1_0.txt">
        <img src="https://img.shields.io/badge/license-BSL%201.0-blue.svg" alt="Boost Software License 1.0"></a>
    <a href="https://godbolt.org/z/45rjY39Ga" alt="godbolt">
        <img src="https://img.shields.io/badge/try%20it%20on-godbolt-green" /></a>
</p>

ckormanyos/serial_win32api implements a modern, C++14, single-file,
header-only serial (COM) driver.

This serial (COM) driver is designed for use with the classic Win32-API
in MSVC. The library is intended to retain compatibility
with C++ language standards C++14, 17, 20, 23 and beyond.

## Usage

Using ckormanyos/serial_win32api is straightforward.
  - Include the header `<serial_win32api.h>`.
  - Prior to inclusion of the header, of course, ensure that the header's path is included in the compiler's include paths.
  - Use `serial_win32api`'s  constructor, its `send()` and `recv()` functions, and other public class methods (see example below).

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

This work has been modernized in 2023. It has been refactored
as header-only to be used with C++ language standards C++14, 17, 20, 23 and beyond.

The original implementation was written in a very old dialect
of C++, as it was created in 1996-98. This legacy work was, however,
_good_ _enough_ to be modernized with ease.

This evolution proves in a small but significant way how
portable, well-designed code can readily
be adapted and remain _good_ _enough_ to be used
for a long time.
