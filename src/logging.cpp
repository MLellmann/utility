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

#include "logging.h"

std::shared_ptr<utility::logging::logSystem> utility::logging::logSystem::m_the_log = nullptr;

void utility::logging::logSystem::add_output_stream(std::shared_ptr<std::ostream> out) {
  if (out != nullptr) {
    m_output_streams.push_back(out);
  }
}

void utility::logging::logSystem::add_output_stream(std::ostream& out) {
  if (out != nullptr) {
    m_output_streams.push_back(std::shared_ptr<std::ostream>(&out, [](std::ostream*){}));
  }
}

bool utility::logging::logSystem::remove_output_stream(std::ostream* out) {
  for (auto p = m_output_streams.begin(); p != m_output_streams.end(); ++p) {
    if (p->get() == out) {
      m_output_streams.erase(p);
      return true;
    }
  }
  return false;
}

void utility::logging::logSystem::flush(void) {
  for (auto& o : m_output_streams ) {
    o->flush();
  }
}

std::shared_ptr<utility::logging::logSystem> utility::logging::logSystem::get_logging_instance(void) {
  if (m_the_log == nullptr) {
    // because the constructor is private, we cant use std::make_shared
    m_the_log = std::shared_ptr<logSystem>(new logSystem());
  }
  return m_the_log;
}

