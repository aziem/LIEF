/* Copyright 2017 R. Thomas
 * Copyright 2017 Quarkslab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <iterator>
#include "LIEF/iostream.hpp"

namespace LIEF {
vector_iostream::vector_iostream(void) = default;

void vector_iostream::reserve(size_t size) {
  this->raw_.reserve(size);
}
vector_iostream& vector_iostream::put(uint8_t c) {

  if (this->raw_.size() < (static_cast<size_t>(this->tellp()) + 1)) {
    this->raw_.resize(static_cast<size_t>(this->tellp()) + 1);
  }
  this->raw_[this->tellp()] = c;
  this->current_pos_ += 1;
  return *this;
}
vector_iostream& vector_iostream::write(const uint8_t* s, std::streamsize n) {
  if (this->raw_.size() < (static_cast<size_t>(this->tellp()) + n)) {
    this->raw_.resize(static_cast<size_t>(this->tellp()) + n);
  }

  auto&& it = std::begin(this->raw_);
  std::advance(it, static_cast<size_t>(this->tellp()));
  std::copy(s, s + n, it);

  this->current_pos_ += n;
  return *this;
}

vector_iostream& vector_iostream::write(const std::vector<uint8_t>& s) {
  if (this->raw_.size() < (static_cast<size_t>(this->tellp()) + s.size())) {
    this->raw_.resize(static_cast<size_t>(this->tellp()) + s.size());
  }
  auto&& it = std::begin(this->raw_);
  std::advance(it, static_cast<size_t>(this->tellp()));
  std::copy(std::begin(s), std::end(s), it);

  this->current_pos_ += s.size();
  return *this;
}


vector_iostream& vector_iostream::write(std::vector<uint8_t>&& s) {
  if (this->raw_.size() < (static_cast<size_t>(this->tellp()) + s.size())) {
    this->raw_.resize(static_cast<size_t>(this->tellp()) + s.size());
  }
  auto&& it = std::begin(this->raw_);
  std::advance(it, static_cast<size_t>(this->tellp()));
  std::copy(
      std::make_move_iterator(std::begin(s)),
      std::make_move_iterator(std::end(s)), it);

  this->current_pos_ += s.size();
  return *this;
}


vector_iostream& vector_iostream::get(std::vector<uint8_t>& c) {
  c = this->raw_;
  return *this;
}

vector_iostream& vector_iostream::flush() {
  return *this;
}

const std::vector<uint8_t>& vector_iostream::raw(void) const {
  return this->raw_;
}

// seeks:
vector_iostream::pos_type vector_iostream::tellp(void) {
  return this->current_pos_;
}
vector_iostream& vector_iostream::seekp(vector_iostream::pos_type p) {
  this->current_pos_ = p;
  return *this;
}
vector_iostream& vector_iostream::seekp(vector_iostream::off_type p, std::ios_base::seekdir dir) {
  switch (dir) {
    case std::ios_base::beg:
      {
        this->current_pos_ = p;
        break;
      }


    case std::ios_base::end:
      {
        //this->current_pos_ = p;
        break;
      }


    case std::ios_base::cur:
      {
        this->current_pos_ += p;
        break;
      }

    default:
      {
        break;
      }
  }

  return *this;
}
}

