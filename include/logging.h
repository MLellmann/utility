/*
 * A class for logging.
 *
 * (C) 2014, 2015 Michael Lellmann
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Version 1.1
 */

#if !defined(INCLUDE_LOGGING_H)
#define INCLUDE_LOGGING_H 1

#include "debug.h"
#include "utility.h"

#include <list>
#include <iostream>
#include <memory>

namespace utility { namespace logging {

  class logSystem {
    private:
      std::list<std::shared_ptr<std::ostream>> m_output_streams;

    private:
      logSystem(void) = default;

    public:
      void add_output_stream(std::shared_ptr<std::ostream> out);
      void add_output_stream(std::ostream& out);

      bool remove_output_stream(std::ostream* out);
      
    private:
      static std::shared_ptr<logSystem> m_the_log;

    public:
      static std::shared_ptr<logSystem> get_logging_instance(void);

    public:
      template<typename T>
      logSystem& operator<<(const T& t);

      void flush(void);

    public:
      ~logSystem() = default;
  };

  template<typename T>
  const std::shared_ptr<logSystem>& operator<<(const std::shared_ptr<logSystem>& l, const T& t);

  // prints a argument list type safe to the active logSystem
  template<typename T, typename... Tail>
  void print_log_message(const T& t, const Tail&... tail);
  inline void print_log_message(void) {}

} }

// helping defines
#define LOCATION_OSTREAM "[" << utility::extract_filename(__FILE__) << ":" \
  << __LINE__ << "(" << __FUNCTION__ << ")]"
#define LOG_MESSAGE_BEGIN(NAME) utility::logging::logSystem::get_logging_instance() << NAME \
  << utility::current_time_as_string() << LOCATION_OSTREAM << ": ";
#define LOG_MESSAGE_END utility::logging::logSystem::get_logging_instance() << "\n";
#define MESSAGE(...) utility::logging::print_log_message(__VA_ARGS__);

#define LOG_MESSAGE(...) { LOG_MESSAGE_BEGIN("LOG") MESSAGE(__VA_ARGS__) \
  LOG_MESSAGE_END }

#define CONDITIONAL_LOG_MESSAGE(COND, ...) \
  { if (COND) { LOG_MESSAGE(__VA_ARGS__} }

#define DEBUG_MESSAGE(...) DEBUG_CODE({ LOG_MESSAGE_BEGIN("DEBUG") \
  MESSAGE(__VA_ARGS__) LOG_MESSAGE_END } )

#include "logging.tcc"

#endif

