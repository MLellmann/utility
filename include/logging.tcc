/*
 * Implementation for inline logging functionality
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

// template function definitions from logging.h - Never include this file directly

#if !defined(INCLUDE_LOGGING_TCC)
#define INCLUDE_LOGGING_TCC 1

#if !defined(INCLUDE_LOGGING_H)
#error "Do not include \"logging.tcc\" directly. Always include \"logging.h\" instead."
#endif

template<typename T>
utility::logging::logSystem& utility::logging::logSystem::operator<<(const T& t) {
  for (auto& o : m_output_streams ) {
    (*o) << t;
  }
  return *this;
}

template<typename T>
const std::shared_ptr<utility::logging::logSystem>& utility::logging::operator<<(
    const std::shared_ptr<utility::logging::logSystem>& l, const T& t) {
  (*l) << t;
  return l;
}

template<typename T, typename... Tail>
void utility::logging::print_log_message(const T& t, const Tail&... tail) {
  utility::logging::logSystem::get_logging_instance() << t;
  print_log_message(tail...);
}

#endif

