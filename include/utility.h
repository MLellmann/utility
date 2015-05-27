/*
 * Utility functions
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

#if !defined(INCLUDE_UTILITY_H)
#define INCLUDE_UTILITY_H 1

#include <string>
#include <sstream>

namespace utility {

  // returns the current time as a string in the format \"[HH:MM:SS]\".
  std::string current_time_as_string(void);

  // returns the filename from a path
  std::string extract_filename(const std::string& path);

  // converts any object to a string with operator <<
  template<typename T>
  inline std::string to_string(const T& t) {
    std::stringstream ss{};
    ss << t;
    return ss.str();
  }

  inline std::string to_string(const std::string& t) { return t; }
  inline std::string to_string(const char* t) { return std::string(t); }

  std::string trim(const std::string& s);
  std::string trim_left(const std::string& s);
  std::string trim_right(const std::string& s);
} 

#endif

