//
// impl/system_executor.ipp
// ~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IMPL_SYSTEM_EXECUTOR_IPP
#define NET_TS_IMPL_SYSTEM_EXECUTOR_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/system_executor.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

struct system_executor::thread_function
{
  detail::scheduler* scheduler_;

  void operator()()
  {
    std::error_code ec;
    scheduler_->run(ec);
  }
};

system_executor::context_impl::context_impl()
  : scheduler_(use_service<detail::scheduler>(*this))
{
  scheduler_.work_started();

  thread_function f = { &scheduler_ };
  std::size_t num_threads = detail::thread::hardware_concurrency() * 2;
  threads_.create_threads(f, num_threads ? num_threads : 2);
}

system_executor::context_impl::~context_impl()
{
  scheduler_.work_finished();
  scheduler_.stop();
  threads_.join();
}

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_IMPL_SYSTEM_EXECUTOR_IPP
