//
// ip/impl/network_v4.ipp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2014 Oliver Kowalke (oliver dot kowalke at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IP_IMPL_NETWORK_V4_IPP
#define NET_TS_IP_IMPL_NETWORK_V4_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <experimental/__net_ts/error.hpp>
#include <experimental/__net_ts/detail/throw_error.hpp>
#include <experimental/__net_ts/detail/throw_exception.hpp>
#include <experimental/__net_ts/ip/network_v4.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace ip {

network_v4::network_v4(const address_v4& addr, unsigned short prefix_len)
  : address_(addr),
    prefix_length_(prefix_len)
{
  if (prefix_len > 32)
  {
    std::out_of_range ex("prefix length too large");
    std::experimental::net::detail::throw_exception(ex);
  }
}

network_v4::network_v4(const address_v4& addr, const address_v4& mask)
  : address_(addr),
    prefix_length_(0)
{
  address_v4::bytes_type mask_bytes = mask.to_bytes();
  bool finished = false;
  for (std::size_t i = 0; i < mask_bytes.size(); ++i)
  {
    if (finished)
    {
      if (mask_bytes[i])
      {
        std::invalid_argument ex("non-contiguous netmask");
        std::experimental::net::detail::throw_exception(ex);
      }
      continue;
    }
    else
    {
      switch (mask_bytes[i])
      {
      case 255:
        prefix_length_ += 8;
        break;
      case 254: // prefix_length_ += 7
        prefix_length_ += 1;
      case 252: // prefix_length_ += 6
        prefix_length_ += 1;
      case 248: // prefix_length_ += 5
        prefix_length_ += 1;
      case 240: // prefix_length_ += 4
        prefix_length_ += 1;
      case 224: // prefix_length_ += 3
        prefix_length_ += 1;
      case 192: // prefix_length_ += 2
        prefix_length_ += 1;
      case 128: // prefix_length_ += 1
        prefix_length_ += 1;
      case 0:   // nbits += 0
        finished = true;
        break;
      default:
        std::out_of_range ex("non-contiguous netmask");
        std::experimental::net::detail::throw_exception(ex);
      }
    }
  }
}

address_v4 network_v4::netmask() const NET_TS_NOEXCEPT
{
  uint32_t nmbits = 0xffffffff;
  if (prefix_length_ == 0)
    nmbits = 0;
  else
    nmbits = nmbits << (32 - prefix_length_);
  return address_v4(nmbits);
}

address_v4_range network_v4::hosts() const NET_TS_NOEXCEPT
{
  return is_host()
    ? address_v4_range(address_, address_v4(address_.to_uint() + 1))
    : address_v4_range(address_v4(network().to_uint() + 1), broadcast());
}

bool network_v4::is_subnet_of(const network_v4& other) const
{
  if (other.prefix_length_ >= prefix_length_)
    return false; // Only real subsets are allowed.
  const network_v4 me(address_, other.prefix_length_);
  return other.canonical() == me.canonical();
}

std::string network_v4::to_string() const
{
  std::error_code ec;
  std::string addr = to_string(ec);
  std::experimental::net::detail::throw_error(ec);
  return addr;
}

std::string network_v4::to_string(std::error_code& ec) const
{
  ec = std::error_code();
  char prefix_len[16];
#if defined(NET_TS_HAS_SECURE_RTL)
  sprintf_s(prefix_len, sizeof(prefix_len), "/%u", prefix_length_);
#else // defined(NET_TS_HAS_SECURE_RTL)
  sprintf(prefix_len, "/%u", prefix_length_);
#endif // defined(NET_TS_HAS_SECURE_RTL)
  return address_.to_string() + prefix_len;
}

network_v4 make_network_v4(const char* str)
{
  return make_network_v4(std::string(str));
}

network_v4 make_network_v4(const char* str, std::error_code& ec)
{
  return make_network_v4(std::string(str), ec);
}

network_v4 make_network_v4(const std::string& str)
{
  std::error_code ec;
  network_v4 net = make_network_v4(str, ec);
  std::experimental::net::detail::throw_error(ec);
  return net;
}

network_v4 make_network_v4(const std::string& str,
    std::error_code& ec)
{
  std::string::size_type pos = str.find_first_of("/");

  if (pos == std::string::npos)
  {
    ec = std::experimental::net::error::invalid_argument;
    return network_v4();
  }

  if (pos == str.size() - 1)
  {
    ec = std::experimental::net::error::invalid_argument;
    return network_v4();
  }

  std::string::size_type end = str.find_first_not_of("0123456789", pos + 1);
  if (end != std::string::npos)
  {
    ec = std::experimental::net::error::invalid_argument;
    return network_v4();
  }

  return network_v4(make_address_v4(str.substr(0, pos)),
      std::atoi(str.substr(pos + 1).c_str()));
}

#if defined(NET_TS_HAS_STD_STRING_VIEW)

network_v4 make_network_v4(string_view str)
{
  return make_network_v4(static_cast<std::string>(str));
}

network_v4 make_network_v4(string_view str,
    std::error_code& ec)
{
  return make_network_v4(static_cast<std::string>(str), ec);
}

#endif // defined(NET_TS_HAS_STD_STRING_VIEW)

} // namespace ip
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_IP_IMPL_NETWORK_V4_IPP
