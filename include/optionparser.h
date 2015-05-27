/*
 * Parsing for command line options
 *
 * (C) 2014, 2015 Michael Lellmann. All rights reserved.
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
 * Version 1.2
 */

#if !defined(INCLUDE_OPTION_PARSER_H)
#define INCLUDE_OPTION_PARSER_H 1

#include "debug.h"
#include "exception.h"

#include <functional>
#include <map>
#include <string>

/*
 * Command line options are supposed to be either
 *   * one char ones preceded by one -
 *   * multi char ones preceded by two --
 *
 * multiple one char options can be combined into one 
 *   eg: -a -b is equal to -ab
 *
 * Options can have one additional parameter
 *   A parameter is a string that is not started with -
 *   A parameter can be added after the option string with an = 
 *   A parameter can be enclosed into quotation marks
 *
 *   eg: -a test          --aa test
 *       -a=test          --aa=test
 *       -a="test"        --aa="test"
 *       -a "test"        --aa "test"
 *  each column is equal 
 *
 * For one char options another option is valid:
 *   -atest 
 *   -a"test"
 *   which are equal to -a test
 *   For this, one char options that have a parameter can not be in the 
 *   combination of multiple options, except as the last one.
 *
 * A parameter not started with - or -- is handled by the special
 * non option parameter handler.
 */

namespace utility { namespace optionParser {

  class formatError : public exception::exceptionBase {
    public:
      formatError(std::string reason) : exceptionBase(std::move(reason)) {}
  };

  class parseError : public exception::exceptionBase {
    public:
      parseError(std::string reason) : exceptionBase(std::move(reason)) {}
  };

  class optionParser {
    public:
      optionParser(void) = default;

    public:
      bool parse(int args, const char** argv, bool ignore_first = true);

    private:
      std::map<char, std::function<bool (void)>> m_single_char;
      std::map<std::string, std::function<bool (void)>> m_multi_char;
      std::map<char, 
        std::function<bool (std::string)>> m_single_char_parameter;
      std::map<std::string, 
        std::function<bool (std::string)>> m_multi_char_parameter;
      std::function<bool (std::string)> m_non_option_parameter{nullptr};

    public:
      bool add_option(const std::string& s, 
        std::function<bool (void)> callback);
      bool add_option(const std::string& s, 
        std::function<bool (std::string)> callback);

    private:
      // these functions make sure that an option is only avaible once
      bool add_option_intern(char ch, std::function<bool (void)> callback);
      bool add_option_intern(const std::string& s, 
        std::function<bool (void)> callback);
      bool add_option_intern(char ch, std::function<bool (std::string)> callback);
      bool add_option_intern(const std::string& s, 
        std::function<bool (std::string)> callback);

    public:
      inline bool has_multi_char_option(const std::string& option) const {
        return m_multi_char.find(option) != m_multi_char.end();
      }
      inline bool has_multi_char_parameter_option(const std::string& option) const {
        return m_multi_char_parameter.find(option) != 
          m_multi_char_parameter.end();
      }
      inline bool has_single_char_option(char option) const {
        return m_single_char.find(option) != m_single_char.end();
      }
      inline bool has_single_char_parameter_option(char option) const {
        return m_single_char_parameter.find(option) != 
          m_single_char_parameter.end();
      }
      inline std::function<bool (void)> 
        get_multi_char_option_callback(const std::string& option) 
      {
        return m_multi_char[option];
      }
      inline std::function<bool (std::string)> 
        get_multi_char_parameter_option_callback(const std::string& option) 
      {
        return m_multi_char_parameter[option];
      }
      inline std::function<bool (void)>
        get_single_char_option_callback(char option) 
      {
        return m_single_char[option];
      }
      inline std::function<bool (std::string)>
        get_single_char_parameter_option_callback(char option)
      {
        return m_single_char_parameter[option];
      }

    public:
      inline bool set_non_option_parameter_callback(
        std::function<bool (std::string)> callback) 
      {
        m_non_option_parameter = callback;
        return true;
      }

      inline std::function<bool (std::string)> 
        get_non_option_parameter_callback(void) 
      {
        return m_non_option_parameter;
      }

      // return true if the option was found
      bool remove_option(char ch);
      bool remove_option(const std::string& s);
  };

} }

#endif

