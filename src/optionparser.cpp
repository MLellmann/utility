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

#include "optionparser.h"

#include "logging.h"

using utility::optionParser::optionParser;
using utility::optionParser::parseError;

namespace {

  struct optionParserHelper {
    optionParserHelper(int arc, const char** arv, optionParser* parsin) : 
      argc{arc}, argv{arv}, parsing{parsin}
    {}

    const int argc;
    const char** argv;
    optionParser* parsing;

    int position{};
    std::string current{};

    std::string check_param(const std::string& param) {
      std::string ret{param};
      if (param == "") {
        // No param given
        throw parseError{"Found a \"=\" but not parameter with it."};
      }
      if (param[0] == '\"') {
        if (param[param.length() - 1] != '\"') {
          throw parseError{"Parameter started with '\"' but did not end with '\"'."};
        }
        ret = param.substr(1, param.length() - 2);
      }
      return ret;
    }

    std::string get_next_param(void) {
      if (position >= argc) {
        throw parseError{"No parameter given."};
      }
      std::string param = std::string{argv[position]};
      ++position;
      return check_param(param);
    }

    bool parse_multi_option(void) {
      if (current.length() == 2) {
        // OOPS - we found -- which is not valid
        throw parseError{"\"--\" is not a valid value for a parameter."};
      } else {
        // Find a = 
        auto equal_sign_pos = current.find('=');
        auto option = current.substr(2, equal_sign_pos - 2);

        // Test for non parameter option
        if (parsing->has_multi_char_option(option)) {
          // Found a multi char option with this name
          if (equal_sign_pos != std::string::npos) {
            // we have an equal sign which is not allowed in this place
            throw parseError("Found an unexpected \"=\".");
          }
          if (!(parsing->get_multi_char_option_callback(option))()) {
            return false;
          }
        } else if (parsing->has_multi_char_parameter_option(option)) {
          std::string param{};
          if (equal_sign_pos != std::string::npos) {
            // we have --option=value
            param = current.substr(equal_sign_pos + 1, std::string::npos);
            param = check_param(param);
          } else {
            // we have --option value
            param = get_next_param();
          }
          if (!(parsing->get_multi_char_parameter_option_callback(option))(param)) {
            return false;
          }
        } else {
          // Error - option not found
          throw parseError{std::string{"Unknown parameter found:"} 
                + option + "."};
        }
      }
      return true;
    }

    bool parse_single_option(void) {
      for (std::size_t i = 1; i < current.length(); ++i) {
        char opt = current[i];
        if (parsing->has_single_char_option(opt)) {
          if (!(parsing->get_single_char_option_callback(opt)())) {
            return false;
          }
        } else if (parsing->has_single_char_parameter_option(opt)) {
          std::string param;
          // get parameter
          if (i != current.length() - 1) {
            param = current.substr(i + 1);
            if (param[0] == '=') {
              param = param.substr(1);
            }
            param = check_param(param);
          } else {
            param = get_next_param();
          }
          if (!(parsing->get_single_char_parameter_option_callback(opt)(param))) {
            return false;
          }
          return true;
        } else {
          // ERROR
          throw parseError{std::string{"Unknown option"} + opt + "."};
          return false;
        }
      }
      return true;
    }

    bool parse_option(void) {
      if (current.length() == 1) {
        // OOPS - we found - which is not valid
        throw parseError{"\'-\' is not a valid value for a parameter."};
      } else {
        if (current[1] == '-') {
          // long option
          if (!parse_multi_option()) {
            return false;
          }
        } else {
          // single char option(s)
          if (!parse_single_option()) {
            return false;
          }
        }
      }
      return true;
    }

    bool handle_non_option_parameter(void) {
      if (parsing->get_non_option_parameter_callback()) {
        std::string param = current;
        if (param[0] == '\"') {
          if (param[param.length() - 1] != '\"') {
            throw parseError{"non conforming parameter"};
          }
          param = param.substr(1, param.length() - 2);
        }
        (parsing->get_non_option_parameter_callback())(param);
      } else {
        throw parseError{"non option parameter found but no callback"};
      }
      return true;
    }

    bool start_parsing(void) {
      while (position < argc) {
        current = std::string{argv[position]};
        ++position;

        if (current.length() == 0) {
          // Empty string as parameter .. we ignore it
          continue;
        }
    
        if (current[0] == '-') {
          // We have an option parameter
          if (!parse_option()) { 
            return false;
          }
        } else {
          if (!handle_non_option_parameter()) {
            return false;
          }
        }
      }
      return true;
    }
  };

}

bool optionParser::parse(int argc, const char** argv, bool ignoreFirst) {
  optionParserHelper oph{argc, argv, this};

  if (ignoreFirst) {
    oph.position = 1;
  }

  return oph.start_parsing();
}

bool optionParser::add_option(const std::string& s, 
        std::function<bool (void)> callback) 
{
  if (s.length() == 0) {
    throw formatError{"The option must not be the empty string."};
  }

  if (s.length() == 1) {
    return add_option_intern(s[0], callback);
  } else {
    return add_option_intern(s, callback);
  }
}

bool optionParser::add_option(const std::string& s, 
        std::function<bool (std::string)> callback) 
{
  if (s.length() == 0) {
    throw formatError{"The option must not be the empty string."};
  }

  if (s.length() == 1) {
    return add_option_intern(s[0], callback);
  } else {
    return add_option_intern(s, callback);
  }
}

bool optionParser::add_option_intern(char ch, std::function<bool (void)> callback) 
{
  if (m_single_char_parameter.find(ch) != m_single_char_parameter.end()) {
    m_single_char_parameter.erase(ch);
  }
  m_single_char[ch] = callback;
  return true;
}

bool optionParser::add_option_intern(const std::string& s, std::function<bool (void)> callback) 
{
  if (m_multi_char_parameter.find(s) != m_multi_char_parameter.end()) {
    m_multi_char_parameter.erase(s);
  }
  m_multi_char[s] = callback;
  return true;
}

bool optionParser::add_option_intern(char ch, std::function<bool (std::string)> callback) 
{
  if (m_single_char.find(ch) != m_single_char.end()) {
    m_single_char.erase(ch);
  }
  m_single_char_parameter[ch] = callback;
  return true;
}

bool optionParser::add_option_intern(const std::string& s, std::function<bool (std::string)> callback) 
{
  if (m_multi_char.find(s) != m_multi_char.end()) {
    m_multi_char.erase(s);
  }
  m_multi_char_parameter[s] = callback;
  return true;
}

bool optionParser::remove_option(char ch) {
  if (m_single_char.find(ch) != m_single_char.end()) {
    m_single_char.erase(ch);
    return true;
  }
  if (m_single_char_parameter.find(ch) != m_single_char_parameter.end()) {
    m_single_char_parameter.erase(ch);
    return true;
  }
  return false;
}

bool optionParser::remove_option(const std::string& s) {
  if (m_multi_char.find(s) != m_multi_char.end()) {
    m_multi_char.erase(s);
    return true;
  }
  if (m_multi_char_parameter.find(s) != m_multi_char_parameter.end()) {
    m_multi_char_parameter.erase(s);
    return true;
  }
  return false;
}

