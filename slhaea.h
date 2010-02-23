// SLHAea - another SUSY Les Houches Accord input/output library
// Copyright © 2009-2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

namespace SLHAea {

// auxiliary functions
template<class Target, class Source> inline Target
to_(const Source& arg)
{ return boost::lexical_cast<Target>(arg); }

template<class Source> inline std::string
to_string(const Source& arg)
{ return boost::lexical_cast<std::string>(arg); }

template<class T> inline std::vector<std::string>
to_string_vector(const std::vector<T>& cont)
{
  std::vector<std::string> vec;
  for (typename std::vector<T>::const_iterator it = cont.begin();
       it != cont.end(); ++it) vec.push_back(to_string(*it));
  return vec;
}

inline std::vector<std::string>
to_string_vector(const std::string& str)
{
  std::vector<std::string> vec;
  boost::split(vec, str, boost::is_space(), boost::token_compress_on);
  return vec;
}

inline std::vector<std::string>
to_string_vector(const std::string& str, const std::string& sep)
{
  std::vector<std::string> vec;
  boost::split(vec, str, boost::is_any_of(sep), boost::token_compress_on);
  return vec;
}

template<class InputIterator> inline std::string
concat(InputIterator first, InputIterator last, const std::string& sep = " ")
{
  std::string retval;
  for (; first != last; ++first) retval += to_string(*first) + sep;
  if (retval.size() > 0) boost::erase_last(retval, sep);
  return retval;
}

template<class Container> inline std::string
concat(const Container& cont, const std::string& sep = " ")
{ return concat(cont.begin(), cont.end(), sep); }


struct SLHAKey
{
  std::string block;
  std::vector<std::string> line;
  std::size_t field;

  SLHAKey(const std::string& _block, const std::vector<std::string>& _line,
    std::size_t _field) : block(_block), line(_line), field(_field) {}

  SLHAKey(const std::string& keyStr)
  { str(keyStr); }

  SLHAKey&
  str(const std::string& keyStr)
  {
    std::vector<std::string> vec = to_string_vector(keyStr, ";");
    if (3 != vec.size())
    { throw std::invalid_argument("SLHAKey::str(\"" + keyStr + "\");"); }

    block = vec[0];
    line  = to_string_vector(vec[1], ",");
    field = to_<std::size_t>(vec[2]);
    return *this;
  }

  std::string
  str() const
  {
    std::stringstream ss("");
    ss << block << ";" << concat(line, ",") << ";" << field;
    return ss.str();
  }
};


// forward declarations
class SLHALine;
class SLHABlock;
class SLHA;

inline std::ostream& operator<<(std::ostream& os, const SLHALine& line);
inline std::ostream& operator<<(std::ostream& os, const SLHABlock& block);
inline std::ostream& operator<<(std::ostream& os, const SLHA& slha);


/**
 * Container of string that represents a line in a %SLHA structure.
 * This class is a container of string that represents a line in a
 * %SLHA structure. The elements of a %SLHALine are the so called
 * fields of an ordinary %SLHA line, which are its
 * whitespace-separated substrings and the comment. For example, if a
 * %SLHALine is constructed from the line <tt>" 1 2 0.123 # a comment
 * "</tt> its elements would be \c "1", \c "2", \c "0.123", and
 * \c "# a comment". Array-style access to the elements with integer
 * indicies is provided by the operator[]() and at() functions.
 *
 * In addition to storing the fields of a %SLHA line, a %SLHALine also
 * stores its formatting (the exact position of the fields in the
 * line). A formatted representation of a %SLHALine can be produced
 * with str() const while str_plain() const produces an unformatted
 * representation where each element is concatenated with a space.
 * The reformat() function clears the previous formatting and indents
 * all elements with a appropriate number of spaces.
 */
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

  /** Constructs an empty %SLHALine. */
  SLHALine() {}

  // NOTE: The compiler-generated copy constructor and assignment
  //   operator for this class are just fine, so we don't need to
  //   write our own.

  /**
   * \brief Constructs %SLHALine from a string.
   * \param line String whose fields are used as content of the
   *   %SLHALine.
   * \sa str()
   */
  SLHALine(const std::string& line)
  { str(line); }

  /**
   * \brief Assigns new content to the %SLHALine based on a string.
   * \param line String whose fields are used as new content of the
   *   %SLHALine.
   * \return Reference to \c *this.
   *
   * This function is an alias for str().
   */
  SLHALine&
  operator=(const std::string& line)
  { return str(line); }

  /**
   * \brief Appends a string to the end of the %SLHALine.
   * \param rhs String that is appended to the %SLHALine.
   * \return Reference to \c *this.
   *
   * This function is an alias for append().
   */
  SLHALine&
  operator+=(const std::string& rhs)
  { return append(rhs); }

  /**
   * \brief Adds an element to the end of the %SLHALine.
   * \param field Element that is added to the end of the %SLHALine.
   * \return Reference to \c *this.
   *
   * This function adds an element to the end of the %SLHALine. If the
   * last element is a comment \p field is only appended to it and
   * thus size() remains unchanged.
   */
  template<class T> SLHALine&
  operator<<(const T& field)
  {
    const std::string rhs = to_string(field);
    const std::string rhs_tr = boost::trim_copy(rhs);
    if (rhs_tr.empty()) return *this;

    if (!empty() && back().find('#') != std::string::npos)
    {
      back() += rhs;
      return *this;
    }

    impl_.push_back(rhs_tr);
    reformat();
    return *this;
  }

  /**
   * \brief Appends a string to the end of the %SLHALine.
   * \param rhs String that is appended to the %SLHALine.
   * \return Reference to \c *this.
   *
   * This functions appends \p rhs to output of str() const and uses
   * this temporary string as input for str(). Based on the temporary
   * string size() is increased or remains unchanged.
   */
  SLHALine&
  append(const std::string& rhs)
  { return str(str() + rhs); }

  /**
   * Returns true if the %SLHALine begins with \c "BLOCK" or
   * \c "DECAY". Comparison is done case-insensitive.
   */
  bool
  is_block_def() const
  {
    return !empty() &&
      (boost::iequals("BLOCK", front()) || boost::iequals("DECAY", front()));
  }

  /** Returns true if the %SLHALine begins with \c "#". */
  bool
  is_comment_line() const
  { return !empty() && ('#' == front()[0]); }

  /**
   * Returns true if the %SLHALine is not empty and if neither
   * is_block_def() nor is_comment_line() returns true.
   */
  bool
  is_data_line() const
  { return !empty() && !is_block_def() && !is_comment_line(); }

  /**
   * \brief Reformats the string representation of the %SLHALine.
   * \return Reference to \c *this.
   */
  SLHALine&
  reformat()
  {
    if (empty()) return *this;

    std::stringstream line_fmt("");
    int arg = 0, pos = 0;
    const_iterator it = begin();

    if (boost::iequals("BLOCK", *it) || boost::iequals("DECAY", *it))
    {
      line_fmt << "%|" << pos << "t|%" << ++arg << "% ";
      pos += it->length();

      if (it+1 != end())
      {
        line_fmt << "%|" << ++pos << "t|%" << ++arg << "% ";
        pos += (++it)->length();
      }
    }
    else if ('#' == (*it)[0])
    {
      line_fmt << "%|" << pos << "t|%" << ++arg << "% ";
      pos += it->length();
    }
    else
    {
      line_fmt << "%|" << ++pos << "t|%" << ++arg << "% ";
      pos += it->length();
    }

    while (++it != end())
    {
      // Compute the number of spaces required for proper indentation.
      int dist = 3 - ((pos - 1) % 4);
      pos += dist > 1 ? dist : dist + 4;

      line_fmt << "%|" << pos << "t|%" << ++arg << "% ";
      pos += it->length();
    }

    lineFormat_ = boost::trim_right_copy(line_fmt.str());
    return *this;
  }

  /**
   * \brief Assigns new content to the %SLHALine based on a string.
   * \param line String whose fields are used as new content of the
   *   %SLHALine.
   * \return Reference to \c *this.
   *
   * This function parses \p line and sets the found fields as new
   * content of the %SLHALine. If \p line contains newlines
   * everything after the first newline is ignored.
   *
   * The exact formatting of \p line is stored internally and can be
   * reproduced with str() const.
   */
  SLHALine&
  str(const std::string& line)
  {
    clear();
    const std::string
      line_tr = boost::trim_copy(line.substr(0, line.find('\n')));
    if (line_tr.empty()) return *this;

    const int comment_pos = std::min(line_tr.find('#'), line_tr.length());
    const std::string
      data    = boost::trim_copy(line_tr.substr(0, comment_pos)),
      comment = boost::trim_copy(line_tr.substr(comment_pos));

    if (!data.empty()) impl_ = to_string_vector(data);
    if (!comment.empty()) impl_.push_back(comment);

    // Construct the format string for line.
    std::stringstream line_fmt("");
    int arg = 0, pos = 0;
    for (const_iterator it = begin(); it != end(); ++it)
    {
      pos = line.find(*it, pos);
      line_fmt << "%|" << pos << "t|%" << ++arg << "% ";
      pos += it->length();
    }
    lineFormat_ = boost::trim_right_copy(line_fmt.str());

    return *this;
  }

  /** Returns a formatted string representation of the %SLHALine. */
  std::string
  str() const
  {
    boost::format fmter(lineFormat_);
    for (const_iterator it = begin(); it != end(); ++it) fmter % *it;
    return fmter.str();
  }

  /** Returns all strings of the %SLHALine concatenated with a space. */
  std::string
  str_plain() const
  { return concat(impl_); }

  // element access
  /**
   * \brief Subscript access to the strings contained in the
   *   %SLHALine.
   * \param n The index of the string which should be accessed.
   * \return Read/write reference to the accessed string.
   *
   * This operator allows for easy, array-style, data access. Note
   * that data access with this operator is unchecked and out_of_range
   * lookups are not defined. (For checked lookups see at().)
   */
  reference
  operator[](size_type n)
  { return impl_[n]; }

  /**
   * \brief Subscript access to the strings contained in the
   *   %SLHALine.
   * \param n The index of the string which should be accessed.
   * \return Read-only (constant) reference to the accessed string.
   *
   * This operator allows for easy, array-style, data access. Note
   * that data access with this operator is unchecked and out_of_range
   * lookups are not defined. (For checked lookups see at().)
   */
  const_reference
  operator[](size_type n) const
  { return impl_[n]; }

  /**
   * \brief Provides access to the strings contained in the %SLHALine.
   * \param n The index of the string which should be accessed.
   * \return Read/write reference to the accessed string.
   * \throw std::out_of_range If \p n is an invalid index.
   */
  reference
  at(size_type n)
  { return impl_.at(n); }

  /**
   * \brief Provides access to the strings contained in the %SLHALine.
   * \param n The index of the string which should be accessed.
   * \return Read-only (constant) reference to the accessed string.
   * \throw std::out_of_range If \p n is an invalid index.
   */
  const_reference
  at(size_type n) const
  { return impl_.at(n); }

  /**
   * Returns a read/write reference to the first element of the
   * %SLHALine.
   */
  reference
  front()
  { return impl_.front(); }

  /**
   * Returns a read-only (constant) reference to the first element of
   * the %SLHALine.
   */
  const_reference
  front() const
  { return impl_.front(); }

  /**
   * Returns a read/write reference to the last element of the
   * %SLHALine.
   */
  reference
  back()
  { return impl_.back(); }

  /**
   * Returns a read-only (constant) reference to the last element of
   * the %SLHALine.
   */
  const_reference
  back() const
  { return impl_.back(); }

  // iterators
  /**
   * Returns a read/write iterator that points to the first element in
   * the %SLHALine. Iteration is done in ordinary element order.
   */
  iterator
  begin()
  { return impl_.begin(); }

  /**
   * Returns a read-only (constant) iterator that points to the first
   * element in the %SLHALine. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  begin() const
  { return impl_.begin(); }

  /**
   * Returns a read-only (constant) iterator that points to the first
   * element in the %SLHALine. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  cbegin() const
  { return impl_.begin(); }

  /**
   * Returns a read/write iterator that points one past the last
   * element in the %SLHALine. Iteration is done in ordinary element
   * order.
   */
  iterator
  end()
  { return impl_.end(); }

  /**
   * Returns a read-only (constant) iterator that points one past the
   * last element in the %SLHALine. Iteration is done in ordinary
   * element order.
   */
  const_iterator
  end() const
  { return impl_.end(); }

  /**
   * Returns a read-only (constant) iterator that points one past the
   * last element in the %SLHALine. Iteration is done in ordinary
   * element order.
   */
  const_iterator
  cend() const
  { return impl_.end(); }

  /**
   * Returns a read/write reverse iterator that points to the last
   * element in the %SLHALine. Iteration is done in reverse element
   * order.
   */
  reverse_iterator
  rbegin()
  { return impl_.rbegin(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * the last element in the %SLHALine. Iteration is done in reverse
   * element order.
   */
  const_reverse_iterator
  rbegin() const
  { return impl_.rbegin(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * the last element in the %SLHALine. Iteration is done in reverse
   * element order.
   */
  const_reverse_iterator
  crbegin() const
  { return impl_.rbegin(); }

  /**
   * Returns a read/write reverse iterator that points to one before
   * the first element in the %SLHALine. Iteration is done in reverse
   * element order.
   */
  reverse_iterator
  rend()
  { return impl_.rend(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * one before the first element in the %SLHALine. Iteration is done
   * in reverse element order.
   */
  const_reverse_iterator
  rend() const
  { return impl_.rend(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * one before the first element in the %SLHALine. Iteration is done
   * in reverse element order.
   */
  const_reverse_iterator
  crend() const
  { return impl_.rend(); }

  // capacity
  /** Returns the number of elements in the %SLHALine. */
  size_type
  size() const
  { return impl_.size(); }

  /**
   * Returns the number of elements without the comment in the
   * %SLHALine.
   */
  size_type
  data_size() const
  {
    size_type s = size();
    if (s > 0 && '#' == back()[0]) --s;
    return s;
  }

  /** Returns the size() of the largest possible %SLHALine. */
  size_type
  max_size() const
  { return impl_.max_size(); }

  /** Returns true if the %SLHALine is empty. */
  bool
  empty() const
  { return impl_.empty(); }

  // modifiers
  /**
   * \brief Swaps data with another %SLHALine.
   * \param line %SLHALine to be swapped with.
   */
  void
  swap(SLHALine& line)
  {
    impl_.swap(line.impl_);
    lineFormat_.swap(line.lineFormat_);
  }

  /** Erases all the elements in the %SLHALine. */
  void
  clear()
  {
    impl_.clear();
    lineFormat_.clear();
  }

private:
  impl_type impl_;
  std::string lineFormat_;
};


/**
 * Container of SLHALine that resembles a block in a %SLHA structure.
 * This class is a named container of SLHALine that resembles a block
 * in a %SLHA structure. In contrast to a block in a %SLHA structure,
 * a %SLHABlock can contain zero, one, or more SLHALine that are
 * block definitions or it can be completely empty.
 */
class SLHABlock
{
private:
  typedef std::vector<SLHALine> impl_type;

public:
  typedef std::vector<std::string>          key_type;
  typedef SLHALine                          value_type;
  typedef SLHALine&                         reference;
  typedef const SLHALine&                   const_reference;
  typedef impl_type::iterator               iterator;
  typedef impl_type::const_iterator         const_iterator;
  typedef impl_type::reverse_iterator       reverse_iterator;
  typedef impl_type::const_reverse_iterator const_reverse_iterator;
  typedef impl_type::difference_type        difference_type;
  typedef impl_type::size_type              size_type;

  /**
   * \brief Constructs an empty %SLHABlock.
   * \param name Name of the %SLHABlock.
   */
  explicit SLHABlock(const std::string& name = "") : name_(name) {}

  // NOTE: The compiler-generated copy constructor and assignment
  //   operator for this class are just fine, so we don't need to
  //   write our own.

  SLHABlock&
  name(const std::string& newName)
  {
    name_ = newName;
    return *this;
  }

  /** Returns the name of the %SLHABlock. */
  const std::string&
  name() const
  { return name_; }

  SLHABlock&
  str(const std::string& block)
  {
    std::string line;
    std::stringstream ss(block);

    clear();
    while (std::getline(ss, line)) push_back(line);
    return *this;
  }

  std::string
  str() const
  {
    std::stringstream ss("");
    ss << *this;
    return ss.str();
  }

  // element access
  reference
  operator[](const key_type& keys)
  {
    iterator it = find(keys);
    if (end() == it)
    {
      push_back(SLHALine());
      return back();
    }
    return *it;
  }

  reference
  operator[](const std::vector<int>& keys)
  { return (*this)[to_string_vector(keys)]; }

  reference
  operator[](const std::string& keys)
  { return (*this)[to_string_vector(keys)]; }

  reference
  operator[](int key)
  { return (*this)[to_string_vector(to_string(key))]; }

  reference
  at(const key_type& keys)
  {
    iterator it = find(keys);
    if (end() == it)
    { throw std::out_of_range("SLHABlock::at(\"" + concat(keys) + "\")"); }
    return *it;
  }

  const_reference
  at(const key_type& keys) const
  {
    const_iterator it = find(keys);
    if (end() == it)
    { throw std::out_of_range("SLHABlock::at(\"" + concat(keys) + "\")"); }
    return *it;
  }

  reference
  at(const std::vector<int>& keys)
  { return at(to_string_vector(keys)); }

  const_reference
  at(const std::vector<int>& keys) const
  { return at(to_string_vector(keys)); }

  reference
  at(const std::string& s0 = "", const std::string& s1 = "",
     const std::string& s2 = "", const std::string& s3 = "")
  {
    std::vector<std::string> keys;

    if (s0.empty()) return at(keys);
    keys.push_back(s0);
    if (s1.empty()) return at(keys);
    keys.push_back(s1);
    if (s2.empty()) return at(keys);
    keys.push_back(s2);
    if (s3.empty()) return at(keys);
    keys.push_back(s3);

    return at(keys);
  }

  const_reference
  at(const std::string& s0 = "", const std::string& s1 = "",
     const std::string& s2 = "", const std::string& s3 = "") const
  {
    std::vector<std::string> keys;

    if (s0.empty()) return at(keys);
    keys.push_back(s0);
    if (s1.empty()) return at(keys);
    keys.push_back(s1);
    if (s2.empty()) return at(keys);
    keys.push_back(s2);
    if (s3.empty()) return at(keys);
    keys.push_back(s3);

    return at(keys);
  }

  reference
  at(int i0, int i1 = nind, int i2 = nind, int i3 = nind)
  {
    std::vector<std::string> keys;

    if (nind == i0) return at(keys);
    keys.push_back(to_string(i0));
    if (nind == i1) return at(keys);
    keys.push_back(to_string(i1));
    if (nind == i2) return at(keys);
    keys.push_back(to_string(i2));
    if (nind == i3) return at(keys);
    keys.push_back(to_string(i3));

    return at(keys);
  }

  const_reference
  at(int i0, int i1 = nind, int i2 = nind, int i3 = nind) const
  {
    std::vector<std::string> keys;

    if (nind == i0) return at(keys);
    keys.push_back(to_string(i0));
    if (nind == i1) return at(keys);
    keys.push_back(to_string(i1));
    if (nind == i2) return at(keys);
    keys.push_back(to_string(i2));
    if (nind == i3) return at(keys);
    keys.push_back(to_string(i3));

    return at(keys);
  }

  /**
   * Returns a read/write reference to the %SLHALine at the first
   * element of the %SLHABlock.
   */
  reference
  front()
  { return impl_.front(); }

  /**
   * Returns a read-only (constant) reference to the %SLHALine at the
   * first element of the %SLHABlock.
   */
  const_reference
  front() const
  { return impl_.front(); }

  /**
   * Returns a read/write reference to the %SLHALine at the last
   * element of the %SLHABlock.
   */
  reference
  back()
  { return impl_.back(); }

  /**
   * Returns a read-only (constant) reference to the %SLHALine at the
   * last element of the %SLHABlock.
   */
  const_reference
  back() const
  { return impl_.back(); }

  // iterators
  /**
   * \brief Tries to locate a SLHALine in the %SLHABlock.
   * \param keys First strings of the SLHALine to be located.
   * \return Read/write iterator pointing to sought-after element, or
   *   end() if not found.
   *
   * This function takes a key (which is a vector of strings) and
   * tries to locate the SLHALine whose first strings are equal to the
   * strings in \p keys. If successful the function returns a
   * read/write iterator pointing to the sought after SLHALine. If
   * unsuccessful it returns end().
   */
  iterator
  find(const key_type& keys)
  {
    if (keys.empty()) return end();
    for (iterator it = begin(); it != end(); ++it)
    {
      if (keys.size() > it->size()) continue;
      if (std::equal(keys.begin(), keys.end(), it->begin())) return it;
    }
    return end();
  }

  /**
   * \brief Tries to locate a SLHALine in the %SLHABlock.
   * \param keys First strings of the SLHALine to be located.
   * \return Read-only (constant) iterator pointing to sought-after
   *   element, or end() const if not found.
   *
   * This function takes a key (which is a vector of strings) and
   * tries to locate the SLHALine whose first strings are equal to the
   * strings in \p keys. If successful the function returns a
   * read-only (constant) iterator pointing to the sought after
   * SLHALine. If unsuccessful it returns end() const.
   */
  const_iterator
  find(const key_type& keys) const
  {
    if (keys.empty()) return end();
    for (const_iterator it = begin(); it != end(); ++it)
    {
      if (keys.size() > it->size()) continue;
      if (std::equal(keys.begin(), keys.end(), it->begin())) return it;
    }
    return end();
  }

  /**
   * Returns a read/write iterator that points to the first element in
   * the %SLHABlock. Iteration is done in ordinary element order.
   */
  iterator
  begin()
  { return impl_.begin(); }

  /**
   * Returns a read-only (constant) iterator that points to the first
   * element in the %SLHABlock. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  begin() const
  { return impl_.begin(); }

  /**
   * Returns a read-only (constant) iterator that points to the first
   * element in the %SLHABlock. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  cbegin() const
  { return impl_.begin(); }

  /**
   * Returns a read/write iterator that points one past the last
   * element in the %SLHABlock. Iteration is done in ordinary element
   * order.
   */
  iterator
  end()
  { return impl_.end(); }

  /**
   * Returns a read-only (constant) iterator that points one past the
   * last element in the %SLHABlock. Iteration is done in ordinary
   * element order.
   */
  const_iterator
  end() const
  { return impl_.end(); }

  /**
   * Returns a read-only (constant) iterator that points one past the
   * last element in the %SLHABlock. Iteration is done in ordinary
   * element order.
   */
  const_iterator
  cend() const
  { return impl_.end(); }

  /**
   * Returns a read/write reverse iterator that points to the last
   * element in the %SLHABlock. Iteration is done in reverse element
   * order.
   */
  reverse_iterator
  rbegin()
  { return impl_.rbegin(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * the last element in the %SLHABlock. Iteration is done in reverse
   * element order.
   */
  const_reverse_iterator
  rbegin() const
  { return impl_.rbegin(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * the last element in the %SLHABlock. Iteration is done in reverse
   * element order.
   */
  const_reverse_iterator
  crbegin() const
  { return impl_.rbegin(); }

  /**
   * Returns a read/write reverse iterator that points to one before
   * the first element in the %SLHABlock. Iteration is done in reverse
   * element order.
   */
  reverse_iterator
  rend()
  { return impl_.rend(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * one before the first element in the %SLHABlock. Iteration is done
   * in reverse element order.
   */
  const_reverse_iterator
  rend() const
  { return impl_.rend(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * one before the first element in the %SLHABlock. Iteration is done
   * in reverse element order.
   */
  const_reverse_iterator
  crend() const
  { return impl_.rend(); }

  // capacity
  /** Returns the number of elements in the %SLHABlock. */
  size_type
  size() const
  { return impl_.size(); }

  /** Returns the size() of the largest possible %SLHABlock. */
  size_type
  max_size() const
  { return impl_.max_size(); }

  /** Returns true if the %SLHABlock is empty. */
  bool
  empty() const
  { return impl_.empty(); }

  // modifiers
  /**
   * \brief Adds a SLHALine to the end of the %SLHABlock.
   * \param line SLHALine to be added.
   *
   * This function creates an element at the end of the %SLHABlock and
   * assigns the given \p line to it.
   */
  void
  push_back(const value_type& line)
  { impl_.push_back(line); }

  void
  push_back(const std::string& line)
  { impl_.push_back(SLHALine(line)); }

  /**
   * Removes the last element. This function shrinks the size() of the
   * %SLHABlock by one.
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
   * thus shorten the %SLHABlock by one.
   */
  iterator
  erase(iterator position)
  { return impl_.erase(position); }

 /**
   * \brief Removes a range of elements.
   * \param first Iterator pointing to the first element to be erased.
   * \param last Iterator pointing to one past the last element to be
   *   erased.
   * \return An iterator pointing to the element pointed to by \p last
   *   prior to erasing (or end()).
   *
   * This function will erase the elements in the range [\p first,
   * \p last) and shorten the %SLHABlock accordingly.
   */
  iterator
  erase(iterator first, iterator last)
  { return impl_.erase(first, last); }

  /**
   * \brief Swaps data with another %SLHABlock.
   * \param block %SLHABlock to be swapped with.
   */
  void
  swap(SLHABlock& block)
  {
    name_.swap(block.name_);
    impl_.swap(block.impl_);
  }

  /**
   * Erases all the elements in the %SLHABlock and set its name to an
   * empty string.
   */
  void
  clear()
  {
    name_.clear();
    impl_.clear();
  }

private:
  std::string name_;
  impl_type impl_;
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
  //   operator for this class are just fine, so we don't need to
  //   write our own.

  explicit SLHA(std::istream& is)
  { read(is); }

  SLHALine::reference
  field(const SLHAKey& key)
  { return at(key.block).at(key.line).at(key.field); }

  SLHALine::const_reference
  field(const SLHAKey& key) const
  { return at(key.block).at(key.line).at(key.field); }

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
      if (line_slha.is_block_def() && line_slha.size() > 1)
      {
        if ('#' != line_slha[1][0]) curr_name = line_slha[1];
      }
      (*this)[curr_name].push_back(line_slha);
    }
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
  {
    std::stringstream ss("");
    ss << slhaStr;
    return read(ss);
  }

  /** Returns a string representation of the %SLHA container. */
  std::string
  str() const
  {
    std::stringstream ss("");
    ss << *this;
    return ss.str();
  }

  // element access
  /**
   * \brief Subscript access to an element of the %SLHA container.
   * \param blockName Name of the SLHABlock to be retrieved.
   * \return Read/write reference to the SLHABlock with the name
   *   \p blockName.
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
    {
      push_back(SLHABlock(blockName));
      return back();
    }
    return *it;
  }

  /**
   * \brief Provides access to an element of the %SLHA container.
   * \param blockName Name of the SLHABlock to be retrieved.
   * \return Read/write reference to the SLHABlock with the name
   *   \p blockName.
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
   * \brief Provides access to an element of the %SLHA container.
   * \param blockName Name of the SLHABlock to be retrieved.
   * \return Read-only (constant) reference to the SLHABlock with the
   *   name \p blockName.
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
   * Returns a read/write iterator that points to the first element in
   * the %SLHA container. Iteration is done in ordinary element order.
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

  // operations
  size_type
  count(const key_type& blockName) const
  { return (find(blockName) != end()) ? 1 : 0; }

  /**
   * \brief Tries to locate a SLHABlock in the %SLHA container.
   * \param blockName Name of the SLHABlock to be located.
   * \return Read/write iterator pointing to sought-after element, or
   *   end() if not found.
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
   *   element, or end() const if not found.
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
   * \brief Adds a SLHABlock to the end of the %SLHA container.
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
   *   erased.
   * \return An iterator pointing to the element pointed to by \p last
   *   prior to erasing (or end()).
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


// stream operators
inline std::istream&
operator>>(std::istream& is, SLHA& slha)
{
  slha.read(is);
  return is;
}

inline std::ostream&
operator<<(std::ostream& os, const SLHAKey& key)
{ return os << key.str(); }

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


// relational operators for SLHALine
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


// relational operators for SLHABlock
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


// relational operators for SLHA
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
