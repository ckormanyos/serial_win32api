///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1998, 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SERIAL_1998_11_23_H
  #define SERIAL_1998_11_23_H

  #include <cstdint>
  #include <limits>
  #include <vector>

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
    auto send(InputIteratorType first, InputIteratorType last) -> bool
    {
      const auto vin = ::std::vector<std::uint8_t>(first, last);

      return this->do_send(vin);
    }

    auto send(const std::uint8_t b) -> bool
    {
      using array_one_byte_type = ::std::array<std::uint8_t, static_cast<std::size_t>(UINT8_C(1))>;

      const auto ar1 = array_one_byte_type { b };

      return send(ar1.cbegin(), ar1.cend());
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

#endif // SERIAL_1998_11_23_H
