/*
 * Assertions
 *
 * (C) 2014 Michael Lellmann. All rights reserved.#
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

#if !defined(INCLUDE_ASSERTION_H)
#define INCLUDE_ASSERTION_H 1

#include "exception.h"
#include "debug.h"
#include "logging.h"

namespace utility { namespace assertion {

  class assertionError : public exception::extendedException {
    public:
      assertionError(std::string filename, int line, std::string function_name, 
        std::string what_msg, const char* assertion)
        : extendedException{std::move(filename), line, std::move(function_name), 
          std::move(what_msg)}, m_assertion(assertion) {}

    public:
      virtual ~assertionError() noexcept = default;

    private:
      std::string m_assertion;

    public:
      std::string get_assertion(void) const noexcept { return m_assertion; }

    public:
      virtual std::ostream& print(std::ostream&) const noexcept override;
  };

} }

#define ASSERTION(COND, MSG) { if (!(COND)) { \
      DEBUG_MESSAGE("Failed Assertion: ", #COND, "\n => Message: " , (MSG), \
        " Condition: ", #COND); \
      THROW(utility::assertion::assertionError, \
        std::string("Failed Assertion: ") + utility::to_string(MSG), #COND); \
    } }

#define DEBUG_ASSERTION(COND, MSG) DEBUG_CODE(ASSERTION((COND), (MSG)))

#endif

