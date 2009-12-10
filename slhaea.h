// SLHAea - another SUSY Les Houches Accord input/output library
// Copyright © 2009 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SLHAEA_H
#define SLHAEA_H

#include <algorithm>
#include <climits>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

namespace SLHAea {

inline bool to_bool(const std::string& str)
{ return boost::lexical_cast<bool>(str); }

inline int to_int(const std::string& str)
{ return boost::lexical_cast<int>(str); }

inline double to_double(const std::string& str)
{ return boost::lexical_cast<double>(str); }

template<class T> inline T to_(const std::string& str)
{ return boost::lexical_cast<T>(str); }

template<class T> inline std::string to_string(const T& v)
{ return boost::lexical_cast<std::string>(v); }

template<class T> inline
std::vector<std::string> to_string_vector(const std::vector<T>& vec)
{
  std::vector<std::string> str_vec;
  for (typename std::vector<T>::const_iterator it = vec.begin();
       it != vec.end(); ++it) str_vec.push_back(to_string(*it));
  return str_vec;
}

inline std::vector<std::string>
to_string_vector(const std::string& str, const std::string& sep = " ")
{
  std::vector<std::string> v;
  if (" " == sep)
  { boost::split(v, str, boost::is_space(), boost::token_compress_on); }
  else
  { boost::split(v, str, boost::is_any_of(sep), boost::token_compress_on); }
  return v;
}

inline std::string concat_strings(const std::vector<std::string>& vec,
                                  const std::string& sep = " ")
{
  std::string str;
  for (std::vector<std::string>::const_iterator it = vec.begin();
       it != vec.end(); ++it) str += *it + sep;
  if (str.size() > 0) boost::erase_last(str, sep);
  return str;
}

class SLHALine;
class SLHABlock;
class SLHA;

std::ostream& operator<<(std::ostream& os, const SLHALine& line);
std::ostream& operator<<(std::ostream& os, const SLHABlock& block);
std::ostream& operator<<(std::ostream& os, const SLHA& slha);


struct SLHAKey
{
  std::string block;
  std::vector<std::string> line;
  std::size_t element;

  SLHAKey(const std::string& _block, const std::vector<std::string>& _line,
    std::size_t _element) : block(_block), line(_line), element(_element) {}

  SLHAKey(const std::string& keyStr)
  { str(keyStr); }

  SLHAKey& str(const std::string& keyStr)
  {
    std::vector<std::string> v = to_string_vector(keyStr, ";");
    if (3 != v.size())
    { throw std::invalid_argument("SLHAKey::str(\"" + keyStr + "\");"); }

    block = v[0];
    line = to_string_vector(v[1], ",");
    element = to_<std::size_t>(v[2]);
    return *this;
  }

  std::string str() const
  {
    std::stringstream ss("");
    ss << block << ";" << concat_strings(line, ",") << ";" << element;
    return ss.str();
  }
};


class SLHALine
{
public:
  typedef std::vector<std::string>::iterator iterator;
  typedef std::vector<std::string>::const_iterator const_iterator;
  typedef std::vector<std::string>::reverse_iterator reverse_iterator;
  typedef std::vector<std::string>::const_reverse_iterator
          const_reverse_iterator;

  SLHALine()
  { str(""); }

  SLHALine(const std::string& line)
  { str(line); }

  SLHALine& operator=(const std::string& line)
  { return str(line); }

  SLHALine& operator+=(const std::string& rhs)
  { return append(rhs); }

  std::string& operator[](std::size_t n)
  { return impl_[n]; }

  const std::string& operator[](std::size_t n) const
  { return impl_[n]; }

  std::string& at(std::size_t n)
  { return impl_.at(n); }

  const std::string& at(std::size_t n) const
  { return impl_.at(n); }

  SLHALine& append(const std::string& rhs)
  { return str(str() + rhs); }

  std::string& back()
  { return impl_.back(); }

  const std::string& back() const
  { return impl_.back(); }

  iterator begin()
  { return impl_.begin(); }

  const_iterator begin() const
  { return impl_.begin(); }

  SLHALine& clear()
  {
    impl_.clear();
    impl_.push_back("");
    mLineFormat = "%1%";
    return *this;
  }

  bool empty() const
  { return size() == 1 && impl_[0] == ""; }

  iterator end()
  { return impl_.end(); }

  const_iterator end() const
  { return impl_.end(); }

  std::string& front()
  { return impl_.front(); }

  const std::string& front() const
  { return impl_.front(); }

  reverse_iterator rbegin()
  { return impl_.rbegin(); }

  const_reverse_iterator rbegin() const
  { return impl_.rbegin(); }

  reverse_iterator rend()
  { return impl_.rend(); }

  const_reverse_iterator rend() const
  { return impl_.rend(); }

  std::size_t size() const
  { return impl_.size(); }

  SLHALine& str(const std::string& line)
  {
    const std::string
      line_tr = boost::trim_copy(line.substr(0, line.find("\n")));
    if (line_tr.empty())
    { clear(); return *this; }

    const int comment_pos = std::min(line_tr.find("#"), line_tr.length());
    const std::string
      data    = boost::trim_copy(line_tr.substr(0, comment_pos)),
      comment = boost::trim_copy(line_tr.substr(comment_pos));

    impl_.clear();
    if (!data.empty())
    {
      boost::split(impl_, data, boost::is_space(), boost::token_compress_on);
    }
    if (!comment.empty()) impl_.push_back(comment);

    // Construct the format string for line.
    std::stringstream line_fmt("");
    int i = 1, pos = 0;
    for (const_iterator it = begin(); it != end(); ++it, ++i, ++pos)
    {
      pos = line.find(*it, pos);
      line_fmt << "%|" << pos << "t|%" << i << "% ";
    }
    mLineFormat = boost::trim_copy(line_fmt.str());

    return *this;
  }

  std::string str() const
  {
    boost::format fmter(mLineFormat);
    for (const_iterator it = begin(); it != end(); ++it) fmter % *it;
    return fmter.str();
  }

  std::string str_plain() const
  {
    std::string retval;
    for (const_iterator it = begin(); it != end(); ++it) retval += *it + " ";
    return boost::trim_copy(retval);
  }

private:
  std::vector<std::string> impl_;
  std::string mLineFormat;
};


class SLHABlock
{
public:
  typedef std::vector<SLHALine>::iterator iterator;
  typedef std::vector<SLHALine>::const_iterator const_iterator;
  typedef std::vector<SLHALine>::reverse_iterator reverse_iterator;
  typedef std::vector<SLHALine>::const_reverse_iterator
          const_reverse_iterator;

  SLHABlock(const std::string& name = "") : mName(name) {}

  SLHALine& operator[](const std::vector<std::string>& keys)
  {
    iterator it = find(keys);
    if (end() == it)
    { push_back(SLHALine()); return back(); }
    return *it;
  }

  const SLHALine& operator[](const std::vector<std::string>& keys) const
  {
    const_iterator it = find(keys);
    if (end() == it)
    {
      throw std::out_of_range("SLHABlock::{operator[],at}(" +
                              concat_strings(keys) + ");");
    }
    return *it;
  }

  SLHALine& operator[](const std::vector<int>& intKeys)
  { return (*this)[to_string_vector<int>(intKeys)]; }

  const SLHALine& operator[](const std::vector<int>& intKeys) const
  { return (*this)[to_string_vector<int>(intKeys)]; }

  SLHALine& operator[](const std::string& keysStr)
  { return (*this)[to_string_vector(keysStr)]; }

  const SLHALine& operator[](const std::string& keysStr) const
  { return (*this)[to_string_vector(keysStr)]; }

  SLHALine& at(const std::string& s0 = "", const std::string& s1 = "",
               const std::string& s2 = "", const std::string& s3 = "")
  {
    std::vector<std::string> keys;

    if (s0.empty()) return (*this)[keys];
    keys.push_back(s0);
    if (s1.empty()) return (*this)[keys];
    keys.push_back(s1);
    if (s2.empty()) return (*this)[keys];
    keys.push_back(s2);
    if (s3.empty()) return (*this)[keys];
    keys.push_back(s3);

    return (*this)[keys];
  }

  const
  SLHALine& at(const std::string& s0 = "", const std::string& s1 = "",
               const std::string& s2 = "", const std::string& s3 = "") const
  {
    std::vector<std::string> keys;

    if (s0.empty()) return (*this)[keys];
    keys.push_back(s0);
    if (s1.empty()) return (*this)[keys];
    keys.push_back(s1);
    if (s2.empty()) return (*this)[keys];
    keys.push_back(s2);
    if (s3.empty()) return (*this)[keys];
    keys.push_back(s3);

    return (*this)[keys];
  }

  SLHALine& at(int i0, int i1 = nind, int i2 = nind, int i3 = nind)
  {
    std::vector<std::string> keys;

    if (nind == i0) return (*this)[keys];
    keys.push_back(to_string(i0));
    if (nind == i1) return (*this)[keys];
    keys.push_back(to_string(i1));
    if (nind == i2) return (*this)[keys];
    keys.push_back(to_string(i2));
    if (nind == i3) return (*this)[keys];
    keys.push_back(to_string(i3));

    return (*this)[keys];
  }

  const
  SLHALine& at(int i0, int i1 = nind, int i2 = nind, int i3 = nind) const
  {
    std::vector<std::string> keys;

    if (nind == i0) return (*this)[keys];
    keys.push_back(to_string(i0));
    if (nind == i1) return (*this)[keys];
    keys.push_back(to_string(i1));
    if (nind == i2) return (*this)[keys];
    keys.push_back(to_string(i2));
    if (nind == i3) return (*this)[keys];
    keys.push_back(to_string(i3));

    return (*this)[keys];
  }

  SLHALine& back()
  { return impl_.back(); }

  const SLHALine& back() const
  { return impl_.back(); }

  iterator begin()
  { return impl_.begin(); }

  const_iterator begin() const
  { return impl_.begin(); }

  SLHABlock& clear()
  { impl_.clear(); return *this; }

  bool empty() const
  { return impl_.empty(); }

  iterator end()
  { return impl_.end(); }

  const_iterator end() const
  { return impl_.end(); }

  iterator erase(iterator position)
  { return impl_.erase(position); }

  iterator erase(iterator first, iterator last)
  { return impl_.erase(first, last); }

  iterator find(const std::vector<std::string>& keys)
  {
    if (keys.empty()) return end();
    for (iterator it = begin(); it != end(); ++it)
    {
      if (keys.size() > it->size()) continue;
      if (std::equal(keys.begin(), keys.end(), it->begin())) return it;
    }
    return end();
  }

  const_iterator find(const std::vector<std::string>& keys) const
  {
    if (keys.empty()) return end();
    for (const_iterator it = begin(); it != end(); ++it)
    {
      if (keys.size() > it->size()) continue;
      if (std::equal(keys.begin(), keys.end(), it->begin())) return it;
    }
    return end();
  }

  SLHALine& front()
  { return impl_.front(); }

  const SLHALine& front() const
  { return impl_.front(); }

  SLHABlock& name(const std::string& newName)
  { mName = newName; return *this; }

  const std::string& name() const
  { return mName; }

  SLHABlock& pop_back()
  { impl_.pop_back(); return *this; }

  SLHABlock& push_back(const SLHALine& line)
  { impl_.push_back(line); return *this; }

  SLHABlock& push_back(const std::string& line)
  { impl_.push_back(SLHALine(line)); return *this; }

  reverse_iterator rbegin()
  { return impl_.rbegin(); }

  const_reverse_iterator rbegin() const
  { return impl_.rbegin(); }

  reverse_iterator rend()
  { return impl_.rend(); }

  const_reverse_iterator rend() const
  { return impl_.rend(); }

  std::size_t size() const
  { return impl_.size(); }

  SLHABlock& str(const std::string& block)
  {
    std::string line;
    std::stringstream ss(block);

    clear();
    while (std::getline(ss, line)) push_back(line);
    return *this;
  }

  std::string str() const
  { std::stringstream ss(""); ss << *this; return ss.str(); }

private:
  std::string mName;
  std::vector<SLHALine> impl_;
  static const int nind = INT_MIN;
};

class SLHA
{
public:
  typedef std::vector<SLHABlock>::iterator iterator;
  typedef std::vector<SLHABlock>::const_iterator const_iterator;
  typedef std::vector<SLHABlock>::reverse_iterator reverse_iterator;
  typedef std::vector<SLHABlock>::const_reverse_iterator
          const_reverse_iterator;

  SLHA() {}

  SLHA(const std::string filename)
  { read_file(filename); }

  SLHABlock& operator[](const std::string& blockName)
  {
    iterator it = find(blockName);
    if (end() == it)
    { push_back(SLHABlock(blockName)); return back(); }
    return *it;
  }

  const SLHABlock& operator[](const std::string& blockName) const
  {
    const_iterator it = find(blockName);
    if (end() == it)
    { throw std::out_of_range("SLHA::operator[](\"" + blockName + "\")"); }
    return *it;
  }

  std::string& at(const SLHAKey& key)
  { return (*this)[key.block][key.line].at(key.element); }

  const std::string& at(const SLHAKey& key) const
  { return (*this)[key.block][key.line].at(key.element); }

  SLHABlock& back()
  { return impl_.back(); }

  const SLHABlock& back() const
  { return impl_.back(); }

  iterator begin()
  { return impl_.begin(); }

  const_iterator begin() const
  { return impl_.begin(); }

  SLHA& clear()
  { impl_.clear(); return *this; }

  bool empty() const
  { return impl_.empty(); }

  iterator end()
  { return impl_.end(); }

  const_iterator end() const
  { return impl_.end(); }

  iterator erase(iterator position)
  { return impl_.erase(position); }

  iterator erase(iterator first, iterator last)
  { return impl_.erase(first, last); }

  iterator find(const std::string& blockName)
  {
    for (iterator it = begin(); it != end(); ++it)
    { if (boost::iequals(it->name(), blockName)) return it; }
    return end();
  }

  const_iterator find(const std::string& blockName) const
  {
    for (const_iterator it = begin(); it != end(); ++it)
    { if (boost::iequals(it->name(), blockName)) return it; }
    return end();
  }

  SLHABlock& front()
  { return impl_.front(); }

  const SLHABlock& front() const
  { return impl_.front(); }

  SLHA& pop_back()
  { impl_.pop_back(); return *this; }

  SLHA& push_back(const SLHABlock& block)
  { impl_.push_back(block); return *this; }

  SLHA& read(std::istream& is)
  {
    std::string line_str, curr_name;

    while (std::getline(is, line_str))
    {
      if (boost::trim_copy(line_str).empty()) continue;

      const SLHALine line_slha(line_str);
      if ((boost::iequals("BLOCK", line_slha[0]) ||
           boost::iequals("DECAY", line_slha[0])) && line_slha.size() > 1)
      {
        if ('#' != line_slha[1][0]) curr_name = line_slha[1];
      }
      (*this)[curr_name][""] = line_slha;
    }
    return *this;
  }

  SLHA& read_file(const std::string& filename)
  {
    std::ifstream fs(filename.c_str());
    if (!fs)
    {
      std::cerr << "SLHA::read_file(\"" << filename << "\") failed"
                << std::endl;
      return *this;
    }
    return read(fs);
  }

  reverse_iterator rbegin()
  { return impl_.rbegin(); }

  const_reverse_iterator rbegin() const
  { return impl_.rbegin(); }

  reverse_iterator rend()
  { return impl_.rend(); }

  const_reverse_iterator rend() const
  { return impl_.rend(); }

  std::size_t size() const
  { return impl_.size(); }

  SLHA& str(const std::string& slhaStr)
  { std::stringstream ss(""); ss << slhaStr; return read(ss); }

  std::string str() const
  { std::stringstream ss(""); ss << *this; return ss.str(); }

  SLHA& write_file(const std::string& filename)
  {
    std::ofstream fs(filename.c_str());
    if (!fs)
    {
      std::cerr << "Error: SLHA::write_file(\"" << filename << "\") failed"
                << std::endl;
      return *this;
    }
    fs << *this;
    return *this;
  }

private:
  std::vector<SLHABlock>  impl_;
};


inline std::ostream& operator<<(std::ostream& os, const SLHA& slha)
{
  for (SLHA::const_iterator it = slha.begin(); it != slha.end(); ++it)
  { os << *it; }
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const SLHABlock& block)
{
  for (SLHABlock::const_iterator it = block.begin(); it != block.end(); ++it)
  { os << *it << std::endl; }
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const SLHALine& line)
{ return os << line.str(); }

inline std::ostream& operator<<(std::ostream& os, const SLHAKey& key)
{ return os << key.str(); }

} // namespace SLHAea

#endif // SLHAEA_H

// vim: sw=2 tw=78
