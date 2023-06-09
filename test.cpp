///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1998, 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

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
