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
  std::size_t field;

  SLHAKey(const std::string& _block, const std::vector<std::string>& _line,
    std::size_t _field) : block(_block), line(_line), field(_field) {}

  SLHAKey(const std::string& keyStr)
  { str(keyStr); }

  SLHAKey& str(const std::string& keyStr)
  {
    std::vector<std::string> vec = to_string_vector(keyStr, ";");
    if (3 != vec.size())
    { throw std::invalid_argument("SLHAKey::str(\"" + keyStr + "\");"); }

    block = vec[0];
    line  = to_string_vector(vec[1], ",");
    field = to_<std::size_t>(vec[2]);
    return *this;
  }

  std::string str() const
  {
    std::stringstream ss("");
    ss << block << ";" << concat_strings(line, ",") << ";" << field;
    return ss.str();
  }
};

inline std::ostream& operator<<(std::ostream& os, const SLHAKey& key)
{ return os << key.str(); }


class SLHALine
{
private:
  typedef std::vector<std::string> impl_type;

public:
  typedef std::string                       value_type;
  typedef std::string&                      reference;
  typedef const std::string&                const_reference;
  typedef impl_type::iterator               iterator;
  typedef impl_type::const_iterator         const_iterator;
  typedef impl_type::reverse_iterator       reverse_iterator;
  typedef impl_type::const_reverse_iterator const_reverse_iterator;
  typedef impl_type::difference_type        difference_type;
  typedef impl_type::size_type              size_type;

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

  template<class T> SLHALine& operator<<(const T& element)
  {
    const std::string rhs = to_string(element);
    const std::string rhs_tr = boost::trim_copy(rhs);
    if (rhs_tr.empty()) return *this;

    if (empty()) front() = rhs_tr;
    else if (back().find("#") != std::string::npos) back() += rhs;
    else impl_.push_back(rhs_tr);

    reformat();
    return *this;
  }

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
  { return size() == 1 && "" == impl_[0]; }

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

  SLHALine& reformat()
  {
    if (empty()) return *this;

    std::string line;
    const_iterator it = begin();

    if (boost::iequals("BLOCK", *it) || boost::iequals("DECAY", *it))
    {
      line += *it;
      if (it+1 != end()) line += " " + *++it;
    }
    else if (it->compare(0, 1, "#") == 0)
    { line += *it; }
    else
    { line += " " + *it; }

    while (++it != end())
    {
      // Compute the number of spaces required for proper indentation.
      int dist = 3 - ((line.size() - 1) % 4);
      int spaces = dist > 1 ? dist : dist + 4;
      line.append(spaces, ' ') += *it;
    }

    str(line);
    return *this;
  }

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
    if (!data.empty()) impl_ = to_string_vector(data);
    if (!comment.empty()) impl_.push_back(comment);

    // Construct the format string for line.
    std::stringstream line_fmt("");
    int i = 1, pos = 0;
    for (const_iterator it = begin(); it != end(); ++it, ++i, ++pos)
    {
      pos = line.find(*it, pos);
      line_fmt << "%|" << pos << "t|%" << i << "% ";
    }
    mLineFormat = boost::trim_right_copy(line_fmt.str());

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
  { return (*this)[to_string_vector(intKeys)]; }

  const SLHALine& operator[](const std::vector<int>& intKeys) const
  { return (*this)[to_string_vector(intKeys)]; }

  SLHALine& operator[](const std::string& keysStr)
  { return (*this)[to_string_vector(keysStr)]; }

  const SLHALine& operator[](const std::string& keysStr) const
  { return (*this)[to_string_vector(keysStr)]; }

  SLHALine& operator[](int intKey)
  { return (*this)[to_string_vector(to_string(intKey))]; }

  const SLHALine& operator[](int intKey) const
  { return (*this)[to_string_vector(to_string(intKey))]; }

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
private:
  typedef std::vector<SLHABlock> impl_type;

public:
  typedef std::string                       key_type;
  typedef SLHABlock                         value_type;
  typedef SLHABlock&                        reference;
  typedef const SLHABlock&                  const_reference;
  typedef impl_type::iterator               iterator;
  typedef impl_type::const_iterator         const_iterator;
  typedef impl_type::reverse_iterator       reverse_iterator;
  typedef impl_type::const_reverse_iterator const_reverse_iterator;
  typedef impl_type::difference_type        difference_type;
  typedef impl_type::size_type              size_type;

  /** Constructs an empty %SLHA container. */
  SLHA() {}

  // NOTE: The compiler-generated copy constructor and assignment
  //       operator for this class are just fine, so we don't need to
  //       write our own.
  // SLHA(const SLHA&);
  // SLHA& operator=(const SLHA&);

  /**
   * \brief Constructs %SLHA container from data provided in a file.
   * \param filename Name of input file to read data from.
   * \sa read_file()
   */
  SLHA(const std::string filename)
  { read_file(filename); }

  /**
   * \brief Transforms data from a stream into the %SLHA container.
   * \param is Input stream to read data from.
   * \returns Reference to \c *this.
   *
   * This function reads lines from \p is, transforms them into
   * SLHALine objects, which are collected into the corresponding
   * SLHABlock objects that are added to this %SLHA container.
   */
  SLHA&
  read(std::istream& is)
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

  /**
   * \brief Transforms data from a file into the %SLHA container.
   * \param filename Name of input file to read data from.
   * \returns Reference to \c *this.
   * \sa read()
   */
  SLHA&
  read_file(const std::string& filename)
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

  /**
   * \brief Writes string representation of the %SLHA container into
   *        a file.
   * \param filename Name of output file the string representation is
   *                 written to.
   * \returns Reference to \c *this.
   */
  SLHA&
  write_file(const std::string& filename)
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

  /**
   * \brief Transforms data from a string into the %SLHA container.
   * \param slhaStr String to read data from.
   * \returns Reference to \c *this.
   * \sa read()
   */
  SLHA&
  str(const std::string& slhaStr)
  { std::stringstream ss(""); ss << slhaStr; return read(ss); }

  /** Returns a string representation of the %SLHA container. */
  std::string
  str() const
  { std::stringstream ss(""); ss << *this; return ss.str(); }

  // element access
  /**
   * \brief Subscript access to an element of the %SLHA container.
   * \param blockName Name of the SLHABlock to be retrieved.
   * \return Read/write reference to the SLHABlock with the name
   *         \p blockName.
   *
   * This function allows for easy lookup with the subscript (\c [])
   * operator. It returns the first SLHABlock with the name
   * \p blockName. If no such SLHABlock is present, an empty SLHABlock
   * with this name is added to the end of the %SLHA container, which
   * is then returned.
   */
  reference
  operator[](const key_type& blockName)
  {
    iterator it = find(blockName);
    if (end() == it)
    { push_back(SLHABlock(blockName)); return back(); }
    return *it;
  }

  /**
   * \brief Access to an element of the %SLHA container.
   * \param blockName Name of the SLHABlock to be retrieved.
   * \return Read/write reference to the SLHABlock with the name
   *         \p blockName.
   * \throw std::out_of_range If no such SLHABlock is present.
   */
  reference
  at(const key_type& blockName)
  {
    iterator it = find(blockName);
    if (end() == it)
    { throw std::out_of_range("SLHA::at(\"" + blockName + "\")"); }
    return *it;
  }

  /**
   * \brief Access to an element of the %SLHA container.
   * \param blockName Name of the SLHABlock to be retrieved.
   * \return Read-only (constant) reference to the SLHABlock with the
   *         name \p blockName.
   * \throw std::out_of_range If no such SLHABlock is present.
   */
  const_reference
  at(const key_type& blockName) const
  {
    const_iterator it = find(blockName);
    if (end() == it)
    { throw std::out_of_range("SLHA::at(\"" + blockName + "\")"); }
    return *it;
  }

  /**
   * \brief Access to a SLHALine element referenced by a SLHAKey.
   * \param key Reference to the SLHALine element to be retrieved.
   * \return Read/write reference to the SLHALine element \p key
   *         refers to.
   * \throw std::out_of_range If the in \p key referred to SLHABlock,
   *        SLHALine, or element of the SLHALine are not present.
   */
  SLHALine::reference
  at_key(const SLHAKey& key)
  { return this->at(key.block)[key.line].at(key.field); }

  /**
   * \brief Access to a SLHALine element referenced by a SLHAKey.
   * \param key Reference to the SLHALine element to be retrieved.
   * \return Read-only (constant) reference to the SLHALine element
   *         \p key refers to.
   * \throw std::out_of_range If the in \p key referred to SLHABlock,
   *        SLHALine, or element of the SLHALine are not present.
   */
  SLHALine::const_reference
  at_key(const SLHAKey& key) const
  { return this->at(key.block)[key.line].at(key.field); }

  /**
   * Returns a read/write reference to the %SLHABlock at the first
   * element of the %SLHA container.
   */
  reference
  front()
  { return impl_.front(); }

  /**
   * Returns a read-only (constant) reference to the %SLHABlock at the
   * first element of the %SLHA container.
   */
  const_reference
  front() const
  { return impl_.front(); }

  /**
   * Returns a read/write reference to the %SLHABlock at the last
   * element of the %SLHA container.
   */
  reference
  back()
  { return impl_.back(); }

  /**
   * Returns a read-only (constant) reference to the %SLHABlock at the
   * last element of the %SLHA container.
   */
  const_reference
  back() const
  { return impl_.back(); }

  // iterators
  /**
   * \brief Tries to locate a SLHABlock in the %SLHA container.
   * \param blockName Name of the SLHABlock to be located.
   * \return Read/write iterator pointing to sought-after element, or
   *         end() if not found.
   *
   * This function takes a key and tries to locate the SLHABlock whose
   * name matches \p blockName. If successful the function returns a
   * read/write iterator pointing to the sought after SLHABlock. If
   * unsuccessful it returns end().
   */
  iterator
  find(const key_type& blockName)
  {
    for (iterator it = begin(); it != end(); ++it)
    { if (boost::iequals(it->name(), blockName)) return it; }
    return end();
  }

  /**
   * \brief Tries to locate a SLHABlock in the %SLHA container.
   * \param blockName Name of the SLHABlock to be located.
   * \return Read-only (constant) iterator pointing to sought-after
   *         element, or end() const if not found.
   *
   * This function takes a key and tries to locate the SLHABlock whose
   * name matches \p blockName. If successful the function returns a
   * read-only (constant) iterator pointing to the sought after
   * SLHABlock. If unsuccessful it returns end() const.
   */
  const_iterator
  find(const key_type& blockName) const
  {
    for (const_iterator it = begin(); it != end(); ++it)
    { if (boost::iequals(it->name(), blockName)) return it; }
    return end();
  }

  /**
   * Returns a read/write iterator that points to the first element
   * in the %SLHA container. Iteration is done in ordinary element
   * order.
   */
  iterator
  begin()
  { return impl_.begin(); }

  /**
   * Returns a read-only (constant) iterator that points to the first
   * element in the %SLHA container. Iteration is done in ordinary
   * element order.
   */
  const_iterator
  begin() const
  { return impl_.begin(); }

  /**
   * Returns a read-only (constant) iterator that points to the first
   * element in the %SLHA container. Iteration is done in ordinary
   * element order.
   */
  const_iterator
  cbegin() const
  { return impl_.begin(); }

  /**
   * Returns a read/write iterator that points one past the last
   * element in the %SLHA container. Iteration is done in ordinary
   * element order.
   */
  iterator
  end()
  { return impl_.end(); }

  /**
   * Returns a read-only (constant) iterator that points one past the
   * last element in the %SLHA container. Iteration is done in
   * ordinary element order.
   */
  const_iterator
  end() const
  { return impl_.end(); }

  /**
   * Returns a read-only (constant) iterator that points one past the
   * last element in the %SLHA container. Iteration is done in
   * ordinary element order.
   */
  const_iterator
  cend() const
  { return impl_.end(); }

  /**
   * Returns a read/write reverse iterator that points to the last
   * element in the %SLHA container. Iteration is done in reverse
   * element order.
   */
  reverse_iterator
  rbegin()
  { return impl_.rbegin(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * the last element in the %SLHA container. Iteration is done in
   * reverse element order.
   */
  const_reverse_iterator
  rbegin() const
  { return impl_.rbegin(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * the last element in the %SLHA container. Iteration is done in
   * reverse element order.
   */
  const_reverse_iterator
  crbegin() const
  { return impl_.rbegin(); }

  /**
   * Returns a read/write reverse iterator that points to one before
   * the first element in the %SLHA container. Iteration is done in
   * reverse element order.
   */
  reverse_iterator
  rend()
  { return impl_.rend(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * one before the first element in the %SLHA container. Iteration is
   * done in reverse element order.
   */
  const_reverse_iterator
  rend() const
  { return impl_.rend(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * one before the first element in the %SLHA container. Iteration is
   * done in reverse element order.
   */
  const_reverse_iterator
  crend() const
  { return impl_.rend(); }

  // capacity
  /** Returns the number of elements in the %SLHA container. */
  size_type
  size() const
  { return impl_.size(); }

  /** Returns the size() of the largest possible %SLHA container. */
  size_type
  max_size() const
  { return impl_.max_size(); }

  /** Returns true if the %SLHA container is empty. */
  bool
  empty() const
  { return impl_.empty(); }

  // modifiers
  /**
   * \brief Adds \p block to the end of the %SLHA container.
   * \param block SLHABlock to be added.
   *
   * This function creates an element at the end of the %SLHA
   * container and assigns the given \p block to it.
   */
  void
  push_back(const value_type& block)
  { impl_.push_back(block); }

  /**
   * Removes the last element. This function shrinks the size() of the
   * %SLHA container by one.
   */
  void
  pop_back()
  { impl_.pop_back(); }

  /**
   * \brief Removes element at given \p position.
   * \param position Iterator pointing to the element to be erased.
   * \return An iterator pointing to the next element (or end()).
   *
   * This function erases the element at the given \p position and
   * thus shorten the %SLHA container by one.
   */
  iterator
  erase(iterator position)
  { return impl_.erase(position); }

  /**
   * \brief Removes a range of elements.
   * \param first Iterator pointing to the first element to be erased.
   * \param last Iterator pointing to one past the last element to be
   *             erased.
   * \return An iterator pointing to the element pointed to by \p last
   *         prior to erasing (or end()).
   *
   * This function will erase the elements in the range [\p first,
   * \p last) and shorten the %SLHA container accordingly.
   */
  iterator
  erase(iterator first, iterator last)
  { return impl_.erase(first, last); }

  /**
   * \brief Swaps data with another %SLHA container.
   * \param rhs %SLHA container to be swapped with.
   */
  void
  swap(SLHA& rhs)
  { impl_.swap(rhs.impl_); }

  /** Erases all the elements in the %SLHA container. */
  void
  clear()
  { impl_.clear(); }

private:
  impl_type impl_;
};


inline std::ostream&
operator<<(std::ostream& os, const SLHALine& line)
{ return os << line.str(); }

inline std::ostream&
operator<<(std::ostream& os, const SLHABlock& block)
{
  for (SLHABlock::const_iterator it = block.begin(); it != block.end(); ++it)
  { os << *it << std::endl; }
  return os;
}

inline std::ostream&
operator<<(std::ostream& os, const SLHA& slha)
{
  for (SLHA::const_iterator it = slha.begin(); it != slha.end(); ++it)
  { os << *it; }
  return os;
}


inline bool
operator==(const SLHALine& a, const SLHALine& b)
{
  return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin())
    && a.str() == b.str();
}

inline bool
operator<(const SLHALine& a, const SLHALine& b)
{
  return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

inline bool
operator!=(const SLHALine& a, const SLHALine& b)
{ return !(a == b); }

inline bool
operator>(const SLHALine& a, const SLHALine& b)
{ return b < a; }

inline bool
operator<=(const SLHALine& a, const SLHALine& b)
{ return !(b < a); }

inline bool
operator>=(const SLHALine& a, const SLHALine& b)
{ return !(a < b); }


inline bool
operator==(const SLHABlock& a, const SLHABlock& b)
{ return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin()); }

inline bool
operator<(const SLHABlock& a, const SLHABlock& b)
{
  return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

inline bool
operator!=(const SLHABlock& a, const SLHABlock& b)
{ return !(a == b); }

inline bool
operator>(const SLHABlock& a, const SLHABlock& b)
{ return b < a; }

inline bool
operator<=(const SLHABlock& a, const SLHABlock& b)
{ return !(b < a); }

inline bool
operator>=(const SLHABlock& a, const SLHABlock& b)
{ return !(a < b); }


inline bool
operator==(const SLHA& a, const SLHA& b)
{ return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin()); }

inline bool
operator<(const SLHA& a, const SLHA& b)
{
  return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

inline bool
operator!=(const SLHA& a, const SLHA& b)
{ return !(a == b); }

inline bool
operator>(const SLHA& a, const SLHA& b)
{ return b < a; }

inline bool
operator<=(const SLHA& a, const SLHA& b)
{ return !(b < a); }

inline bool
operator>=(const SLHA& a, const SLHA& b)
{ return !(a < b); }

} // namespace SLHAea

#endif // SLHAEA_H

// vim: sw=2 tw=78
