/*
 * Exception handling
 *
 * (C) 2014 Michael Lellmann. All rights reserved.
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

#if !defined(INCLUDE_EXCEPTION_H)
#define INCLUDE_EXCEPTION_H 1

#include "debug.h"
#include "utility.h"

#include <exception>
#include <string>
#include <ostream>

namespace utility { namespace exception {

  class exceptionBase {
    public:
      exceptionBase(std::string reason) : m_reason{std::move(reason)} {}
      virtual ~exceptionBase() noexcept = default;

    private:
      std::string m_reason;

    public:
      virtual std::string get_reason(void) const noexcept { return m_reason; }
      virtual std::ostream& print(std::ostream& os) const noexcept;
  };

  class extendedException : public exceptionBase {
    public:
      extendedException(std::string filename, int line, std::string function_name, 
        std::string what_msg) 
        :  exceptionBase{std::move(what_msg)},
          m_filename{utility::extract_filename(std::move(filename))},
          m_line{line}, m_function_name{std::move(function_name)}
      {}

    private:
      std::string m_filename;
      int m_line;
      std::string m_function_name;

    public:
      std::string get_filename(void) const noexcept { return m_filename; }
      int get_line(void) const noexcept { return m_line; }
      std::string get_function_name(void) const noexcept { return m_function_name; }

    public:
      virtual ~extendedException() noexcept = default;

    public:
      virtual std::ostream& print(std::ostream& os) const noexcept override;
  };

  // Prints the informations of an exception to an ostream
  inline std::ostream& operator<<(std::ostream& os, const extendedException& le) {
    return le.print(os);
  }

} }

#define THROW(TYPE, ...) { DEBUG_MESSAGE("Throwing exception of type: ", #TYPE); \
  throw TYPE(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }

#endif

