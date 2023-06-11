///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1998, 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SERIAL_WIN32_API_1998_11_23_H
  #define SERIAL_WIN32_API_1998_11_23_H

  #include <array>
  #include <cstddef>
  #include <cstdint>
  #include <limits>
  #include <vector>

  #include <windows.h>

  struct t_scb
  {
    explicit t_scb(const std::uint32_t ch,
                   const std::uint32_t bd     = static_cast<std::uint32_t>(UINT32_C(9600)),
                   const std::uint32_t n_send = static_cast<std::uint32_t>(UINT32_C(0x10000)),
                   const std::uint32_t n_recv = static_cast<std::uint32_t>(UINT32_C(0x10000)))
      : channel     (ch),
        baud        (bd),
        send_buf_len(n_send),
        recv_buf_len(n_recv) { }

    t_scb() = delete;

    ~t_scb() = default;

    t_scb(const t_scb&) = default;
    t_scb(t_scb&&) noexcept = default;

    auto operator=(const t_scb&) -> t_scb& = default;
    auto operator=(t_scb&&) noexcept -> t_scb& = default;

    std::uint32_t channel { static_cast<std::uint32_t>(UINT8_C(1)) };
    std::uint32_t baud    { static_cast<std::uint32_t>(UINT16_C(9600)) };

    std::uint32_t send_buf_len { static_cast<std::uint32_t>(UINT32_C(0x10000)) };
    std::uint32_t recv_buf_len { static_cast<std::uint32_t>(UINT32_C(0x10000)) };
  };

  class serial
  {
  public:
    static constexpr int open_Ok                    = 0x00000000;
    static constexpr int open_BadChannelNumber      = 0x00000001;
    static constexpr int open_ChannelInUse          = 0x00000002;
    static constexpr int open_ChannelNotAvailable   = 0x00000004;
    static constexpr int open_NotEnoughMemory       = 0x00000008;
    static constexpr int open_InvalidParams         = 0x00000010;
    static constexpr int open_BadBufferSize         = 0x00000020;
    static constexpr int open_BaudAdjusted          = 0x00000100;
    static constexpr int open_BitsAdjusted          = 0x00000200;
    static constexpr int open_StopAdjusted          = 0x00000400;
    static constexpr int open_ParityAdjusted        = 0x00000800;
    static constexpr int open_ModeAdjusted          = 0x00001000;
    static constexpr int open_ReceiveBufferAdjusted = 0x00002000;
    static constexpr int open_SendBufferAdjusted    = 0x00004000;
    static constexpr int open_Error                 =   open_BadChannelNumber
                                                      | open_ChannelInUse
                                                      | open_ChannelNotAvailable
                                                      | open_NotEnoughMemory
                                                      | open_InvalidParams;

    explicit serial(const std::uint32_t ch,
                    const std::uint32_t bd     = static_cast<std::uint32_t>(UINT16_C(9600)),
                    const std::uint32_t n_send = static_cast<std::uint32_t>(UINT32_C(0x10000)),
                    const std::uint32_t n_recv = static_cast<std::uint32_t>(UINT32_C(0x10000)))
      : m_scb(ch, bd, n_send, n_recv) { }

    serial() = delete;

    serial(const serial&) = delete;
    serial(serial&&) noexcept = delete;

    auto operator=(const serial&) -> serial& = delete;
    auto operator=(serial&&) noexcept -> serial& = delete;

    virtual ~serial() = default;

    virtual auto open(const t_scb& scb, std::uint32_t& result) -> bool = 0;
    virtual auto close() -> bool = 0;

    virtual auto recv(::std::vector<std::uint8_t>& data) const -> std::uint32_t = 0;

    virtual auto send_in_progress() const -> bool = 0;
    virtual auto recv_ready() const -> std::uint32_t = 0;

    auto send(const ::std::vector<std::uint8_t>& data) -> bool
    {
      return this->do_send(data);
    }

    template<typename InputIteratorType>
    auto send_n(InputIteratorType first, InputIteratorType last) -> bool
    {
      const auto vin = ::std::vector<std::uint8_t>(first, last);

      return this->do_send(vin);
    }

    auto send(const std::uint8_t b) -> bool
    {
      using array_one_byte_type = ::std::array<std::uint8_t, static_cast<std::size_t>(UINT8_C(1))>;

      const auto ar1 = array_one_byte_type { b };

      return send_n(ar1.cbegin(), ar1.cend());
    }

    auto set_chan(const std::uint32_t ch) -> bool
    {
      auto result_set_chan_is_ok = bool { };

      if((!m_is_error) && (!m_is_open))
      {
        m_scb.channel = ch;

        result_set_chan_is_ok = true;
      }
      else
      {
        result_set_chan_is_ok = false;
      }

      return result_set_chan_is_ok;
    }

    auto set_baud(const std::uint32_t bd) -> bool
    {
      auto result_set_baud_is_ok = bool { };

      if((!m_is_error) && (!m_is_open))
      {
        m_scb.baud = bd;

        result_set_baud_is_ok = true;
      }
      else
      {
        result_set_baud_is_ok = false;
      }

      return result_set_baud_is_ok;
    }

    auto valid() const -> bool { return (is_open() && (!is_error())); }

  protected:
    t_scb m_scb { (std::numeric_limits<std::uint32_t>::max)() };

    bool m_is_open  { false };
    bool m_is_error { false };

    bool is_open() const { return m_is_open;  }
    bool is_error() const { return m_is_error; }

  private:
    virtual auto do_send(const ::std::vector<std::uint8_t>& data) -> bool = 0;
  };

  class serial_win32api : public serial
  {
  public:
    explicit serial_win32api(const std::uint32_t ch,
                             const std::uint32_t bd     = static_cast<std::uint32_t>(UINT16_C(9600)),
                             const std::uint32_t n_send = static_cast<std::uint32_t>(UINT32_C(0x10000)),
                             const std::uint32_t n_recv = static_cast<std::uint32_t>(UINT32_C(0x10000)))
      : serial(ch, bd, n_send, n_recv)
    {
      auto result_to_get = std::uint32_t { };

      const auto result_open_is_ok = open(m_scb, result_to_get);

      static_cast<void>(result_open_is_ok);
      static_cast<void>(result_to_get);
    }

    ~serial_win32api() override
    {
      if(is_open())
      {
        static_cast<void>(close());
      }
    }

    auto open(const t_scb& scb, std::uint32_t& result_to_get) -> bool override
    {
      auto result_open_is_ok = bool { };

      if(m_is_open)
      {
        result_open_is_ok = false;

        result_to_get = open_ChannelInUse;
      }
      else
      {
        if(do_open(scb, result_to_get))
        {
          m_is_open  = true;
          m_is_error = false;

          result_open_is_ok = (result_to_get == static_cast<std::uint32_t>(UINT8_C(0)));
        }
        else
        {
          m_is_open  = false;
          m_is_error = true;

          result_open_is_ok = false;
        }
      }

      return result_open_is_ok;
    }

    auto close() -> bool override
    {
      auto result_close_is_ok = bool { };

      if(is_open())
      {
        // Reset RTS.
        static_cast<void>(::EscapeCommFunction(my_handle, CLRRTS));

        // Reset DTR.
        static_cast<void>(::EscapeCommFunction(my_handle, CLRDTR));

        // Flush  and close port.
        static_cast<void>
        (
          ::PurgeComm(my_handle,   PURGE_TXABORT
                                 | PURGE_TXCLEAR
                                 | PURGE_RXABORT
                                 | PURGE_RXCLEAR)
        );

        {
          const auto b_close = ::CloseHandle(my_handle);

          result_close_is_ok = (b_close == TRUE);
        }

        my_handle = nullptr;

        m_is_open = false;
      }
      else
      {
        result_close_is_ok = false;
      }

      return result_close_is_ok;
    }

    auto recv(::std::vector<std::uint8_t>& recv_buf) const -> std::uint32_t override
    {
      const auto count_ready = recv_ready();

      auto result = std::uint32_t { };

      if(count_ready != static_cast<std::uint32_t>(UINT8_C(0)))
      {
        // Resize the vector. Release it's buffer first.
        recv_buf = ::std::vector<std::uint8_t> { };

        recv_buf.resize(static_cast<std::size_t>(count_ready));

        auto bytes_read = DWORD { };

        static_cast<void>
        (
          ::ReadFile(my_handle, static_cast<void*>(recv_buf.data()), static_cast<DWORD>(count_ready), &bytes_read, nullptr)
        );

        if(bytes_read < static_cast<DWORD>(count_ready))
        {
          recv_buf.resize(static_cast<std::size_t>(count_ready));
        }

        result = static_cast<std::uint32_t>(recv_buf.size());
      }
      else
      {
        result = static_cast<std::uint32_t>(UINT8_C(0));
      }

      return result;
    }

    auto send_in_progress() const -> bool override
    {
      auto result_send_is_in_progress = bool { };

      if(is_open())
      {
        auto errors = DWORD   { };
        auto stat   = COMSTAT { };

        static_cast<void>(::ClearCommError(my_handle, &errors, &stat));

        result_send_is_in_progress =
          (static_cast<std::uint32_t>(static_cast<std::uint32_t>(stat.cbOutQue)) != static_cast<std::uint32_t>(UINT8_C(0)));
      }
      else
      {
        result_send_is_in_progress = false;
      }

      return result_send_is_in_progress;
    }

    auto recv_ready() const -> std::uint32_t override
    {
      auto count_ready = std::uint32_t { };

      if(is_open())
      {
        auto errors = DWORD   { };
        auto stat   = COMSTAT { };

        static_cast<void>(::ClearCommError(my_handle, &errors, &stat));

        count_ready = static_cast<std::uint32_t>(stat.cbInQue);
      }
      else
      {
        count_ready = static_cast<std::uint32_t>(UINT8_C(0));
      }

      return count_ready;
    }

  private:
    HANDLE my_handle { nullptr };

    constexpr auto ms_for_bytes(std::uint32_t count) -> DWORD
    {
      return
        static_cast<DWORD>
        (
          static_cast<std::uintmax_t>
          (
            static_cast<std::uintmax_t>
            (
              static_cast<std::uintmax_t>
              (
                  static_cast<std::uintmax_t>(m_scb.baud)
                * static_cast<std::uintmax_t>(count)
              )
              / static_cast<std::uintmax_t>(UINT8_C(10))
            )
            / static_cast<std::uintmax_t>(UINT16_C(1000))
          )
        );
    }

    auto do_open(const t_scb& scb, std::uint32_t& result) -> bool
    {
      result = static_cast<std::uint32_t>(UINT8_C(0));

      // Create name of port: "COMn"
      auto str_channel =
        ::std::array<char, static_cast<std::size_t>(UINT8_C(5))>
        {
          'C', 'O', 'M', 'n', '\0'
        };

      str_channel[static_cast<std::size_t>(UINT8_C(3))] =
        static_cast<char>
        (
          scb.channel + static_cast<std::uint32_t>(UINT8_C(0x30))
        );

      my_handle = ::CreateFile(str_channel.data(),
                               static_cast<DWORD>(GENERIC_READ | GENERIC_WRITE),
                               static_cast<DWORD>(UINT8_C(0)),
                               nullptr,
                               static_cast<DWORD>(OPEN_EXISTING),
                               static_cast<DWORD>(UINT8_C(0)),
                               nullptr);

      if(my_handle == INVALID_HANDLE_VALUE)
      {
        const auto dw_error = ::GetLastError();

        switch(dw_error)
        {
          case IE_BADID:
            result |= open_BadChannelNumber;
            break;

          case IE_OPEN:
            result |= open_ChannelInUse;
            break;

          case IE_MEMORY:
            result |= open_NotEnoughMemory;
            break;

          case IE_DEFAULT:
          case IE_BYTESIZE:
          case IE_BAUDRATE:
            result |= open_InvalidParams;
            break;

          case IE_HARDWARE:
          case IE_NOPEN:
            result |= open_ChannelNotAvailable;
            break;
        }

        return false;
      }
      else
      {
        if(::SetupComm(my_handle, scb.recv_buf_len, scb.send_buf_len) == FALSE)
        {
          result |= open_BadBufferSize;

          return false;
        }

        // Setup serial device.
        auto dcb = DCB { };

        std::memset(static_cast<void*>(&dcb), static_cast<int>(INT8_C(0)), sizeof(DCB));

        dcb.DCBlength = static_cast<DWORD>(sizeof(DCB)); // Structure size
        dcb.BaudRate  = static_cast<DWORD>(scb.baud);    // Baud rate
        dcb.ByteSize  = static_cast<BYTE>(UINT8_C(8));   // 8 data bits
        dcb.StopBits  = static_cast<BYTE>(ONESTOPBIT);   // One stop bit
        dcb.fBinary   = static_cast<DWORD>(UINT8_C(1));  // Binary Mode (skip EOF check)

        dcb.fOutxCtsFlow      = static_cast<DWORD>(UINT8_C(0));           // No CTS handshaking on output
        dcb.fRtsControl       = static_cast<DWORD>(RTS_CONTROL_DISABLE);  // Disable Rts Flow control
        dcb.fOutxDsrFlow      = static_cast<DWORD>(UINT8_C(0));           // No DSR handshaking on output
        dcb.fDtrControl       = static_cast<DWORD>(DTR_CONTROL_DISABLE);  // Disable DTR Flow control
        dcb.fDsrSensitivity   = static_cast<DWORD>(UINT8_C(0));           // No DSR Sensitivity
        dcb.fOutX             = static_cast<DWORD>(UINT8_C(0));           // Disable output X-ON/X-OFF
        dcb.fInX              = static_cast<DWORD>(UINT8_C(0));           // Disable input X-ON/X-OFF
        dcb.fTXContinueOnXoff = static_cast<DWORD>(UINT8_C(0));           // Do not continue TX when Xoff sent

        if(!::SetCommState(my_handle, &dcb))
        {
          result |= open_InvalidParams;

          return false;
        }

        // Analyze baud rate.
        const auto needs_baud_adjust =
        (
          static_cast<std::uint32_t>(static_cast<std::uint32_t>(CBR_115200) % scb.baud) != static_cast<std::uint32_t>(UINT8_C(0))
        );

        // Set the serial control block.
        m_scb = scb;

        if(needs_baud_adjust)
        {
          result |= open_BaudAdjusted;
        }

        const volatile auto b_reset =
          ::SetCommMask(my_handle,  EV_TXEMPTY
                                  | EV_CTS
                                  | EV_DSR
                                  | EV_BREAK
                                  | EV_ERR);

        static_cast<void>(b_reset);

        return true;
      }
    }

    auto do_send(const ::std::vector<std::uint8_t>& send_buf) -> bool override
    {
      auto result_send_is_ok = bool { };

      if(is_open())
      {
        if(send_buf.size() == static_cast<std::size_t>(UINT8_C(0)))
        {
          result_send_is_ok = true;
        }
        else
        {
          if(send_in_progress() || (!m_is_open) || m_is_error)
          {
            result_send_is_ok = false;
          }
          else
          {
            if(send_buf.size() < m_scb.send_buf_len)
            {
              // Send the data in one packet and return immediately.
              auto bytes_written = DWORD { };

              // Write the buffer.
              static_cast<void>
              (
                ::WriteFile(my_handle,
                            static_cast<LPCVOID>(send_buf.data()),
                            static_cast<DWORD>(send_buf.size()),
                            &bytes_written,
                            nullptr)
              );

              result_send_is_ok = (bytes_written == static_cast<DWORD>(send_buf.size()));
            }
            else
            {
              // Send the data in packets and return when all of the packets have
              // been sent. The routine waits until even the last packet or the
              // last portion of the last packet packet has been completely transmitted.

              auto number = static_cast<std::uint32_t>(UINT8_C(0));

              do
              {
                auto bytes_written = DWORD { };

                const auto remaining =
                  static_cast<std::uint32_t>
                  (
                    send_buf.size() - static_cast<std::size_t>(number)
                  );

                const auto send_size =
                  static_cast<std::uint32_t>
                  (
                    (remaining > m_scb.send_buf_len) ? m_scb.send_buf_len : remaining
                  );

                static_cast<void>
                (
                  ::WriteFile(my_handle,
                              static_cast<LPCVOID>(send_buf.data() + static_cast<std::size_t>(number)),
                              static_cast<DWORD>(send_size),
                              &bytes_written,
                              nullptr)
                );

                // Set an appropriate timeout.
                const auto timeout =
                  static_cast<ULONGLONG>
                  (
                      ::GetTickCount64()                                // Current tick
                    + static_cast<ULONGLONG>(ms_for_bytes(send_size))   // Expected number of milliseconds for transmission of the bytes
                    + static_cast<ULONGLONG>(UINT16_C(1000))            // 1s extra
                  );

                auto b_timeout = false;

                auto scale = static_cast<std::uint32_t>(UINT8_C(0));

                for(;;)
                {
                  b_timeout = (::GetTickCount64() > timeout);

                  if((!send_in_progress()) || b_timeout)
                  {
                    break;
                  }

                  // Sleep 0ms or, occasionally, 10ms.
                  {
                    const auto use_scale =
                      (static_cast<std::uint32_t>(++scale % static_cast<std::uint32_t>(UINT8_C(0x10))) == static_cast<std::uint32_t>(UINT8_C(0)));

                    const auto sleep_amount =
                      static_cast<DWORD>
                      (
                        use_scale ? static_cast<DWORD>(UINT8_C(10)) : static_cast<DWORD>(UINT8_C(0))
                      );

                    ::Sleep(sleep_amount);
                  }
                }

                number += send_size;

                result_send_is_ok = ((!b_timeout) && (bytes_written == send_size));
              }
              while((number < send_buf.size()) && result_send_is_ok);
            }
          }
        }
      }
      else
      {
        result_send_is_ok = false;
      }

      return result_send_is_ok;
    }
  };

#endif // SERIAL_WIN32_API_1998_11_23_H
