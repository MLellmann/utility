/*
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

#include "utility.h"

#include <ctime>
#include <iostream>
#include <limits>

std::string utility::current_time_as_string(void) {
  std::time_t t = std::time(nullptr);
  struct std::tm now = *std::localtime(&t);
  std::stringstream ss;
  ss << "[";
  ss.fill('0');
  ss.width(2);
  ss << (now.tm_hour) << ":";
  ss.width(2);
  ss << (now.tm_min) << ":";
  ss.width(2);
  ss << (now.tm_sec) << "]";
  return ss.str();
}

std::string utility::extract_filename(const std::string& path) {
  std::string res;
  auto pos = path.find_last_of("\\/");
  if (pos == std::string::npos) {
    res = path;
  } else {
    res = path.substr(pos + 1);
  }
  return res;
}

std::string utility::trim(const std::string &s)
{
  return trim_left(trim_right(s));
}

std::string utility::trim_left(const std::string& s) {
  std::string::size_type trim = 0u;
  while ((trim <= s.length()) && (std::isspace(s[trim]))) {
    ++trim;
  }
  return s.substr(trim);
}

std::string utility::trim_right(const std::string& s) {
  auto trim = s.length() - 1;
  while ((trim > 0) && (std::isspace(s[trim]))) {
    --trim;
  }
  return s.substr(0, trim + 1);
}

