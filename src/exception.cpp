/*
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
 * Version 1.0
 */

#include "exception.h"

std::ostream& utility::exception::exceptionBase::print(std::ostream& os) const noexcept {
  os << "Exception: " << get_reason() << "\n";
  return os;
}

std::ostream& utility::exception::extendedException::print(std::ostream& os) const noexcept {
  os << "EXCEPTION FROM: [" << get_filename() << ":" << get_line() << "(" 
    << get_function_name() << ")]: " << get_reason() << "\n";
  return os;
}

