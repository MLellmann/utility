/*
 * Debugging and Logging
 *
 * (C) 2014 Michael Lellmann
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

#if !defined(INCLUDE_DEBUG_H)
#define INCLUDE_DEBUG_H 1

// If neither DEBUG nor NDEBUG are defined, we got a problem
#if !defined(DEBUG) && !defined(NDEBUG)
#error "Need to define either DEBUG or NDEBUG"
#endif

// If both DEBUG and NDEBUG are defined, we got a problem
#if defined(DEBUG) && defined(NDEBUG)
#error "Need to define either DEBUG or NDEBUG"
#endif

#if defined(DEBUG)
# define DEBUG_MODE 1
#else
# define DEBUG_MODE 0
#endif

#if DEBUG_MODE
# define DEBUG_CODE(CODE) CODE
#else
# define DEBUG_CODE(CODE)
#endif

#endif

