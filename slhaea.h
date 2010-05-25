// SLHAea - another SUSY Les Houches Accord input/output library
// Copyright © 2009-2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SLHAEA_H
#define SLHAEA_H

#include <algorithm>
#include <climits>
#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

namespace SLHAea {

// auxiliary functions
/**
 * \brief Converts an object of type \c Source to an object of type
 *   \c Target.
 * \param arg Object that will be converted.
 * \return Result of the conversion of \p arg to \c Target.
 *
 * This function is a wrapper for \c boost::lexical_cast().
 */
template<class Target, class Source> inline Target
to_(const Source& arg)
{ return boost::lexical_cast<Target>(arg); }

/**
 * \brief Converts an object of type \c Source to a string.
 * \param arg Object that will be converted.
 * \return Result of the conversion of \p arg to \c std::string.
 *
 * This function is an alias for to_<std::string>().
 */
template<class Source> inline std::string
to_string(const Source& arg)
{ return boost::lexical_cast<std::string>(arg); }

/**
 * \brief Converts all elements of a container to strings.
 * \param cont Container whose elements will be converted.
 * \return \c std::vector that contains the converted elements.
 */
template<class Container> inline std::vector<std::string>
cont_to_string_vector(const Container& cont)
{
  std::vector<std::string> result;
  result.reserve(cont.size());
  std::transform(cont.begin(), cont.end(), std::back_inserter(result),
                 to_string<typename Container::value_type>);
  return result;
}

/**
 * \brief Splits a string into tokens separated by white space.
 * \param str String that will be searched for tokens.
 * \return \c std::vector that contains all tokens.
 */
inline std::vector<std::string>
split_string(const std::string& str)
{
  std::vector<std::string> result;
  boost::split(result, str, boost::is_space(), boost::token_compress_on);
  return result;
}

/**
 * \brief Splits a string into tokens separated by a separator.
 * \param str String that will be searched for tokens.
 * \param sep Separator that delimits tokens.
 * \return \c std::vector that contains all tokens.
 */
inline std::vector<std::string>
split_string(const std::string& str, std::string sep)
{
  std::vector<std::string> result;
  boost::split(result, str, boost::is_any_of(sep), boost::token_compress_on);
  return result;
}

/**
 * \brief Joins all elements in an input range into one string.
 * \param first, last Input iterators to the initial and final
 *   positions of the sequence to use.
 * \param sep String that will separate the joined elements.
 * \return Concatenated string.
 */
template<class InputIterator> inline std::string
join(InputIterator first, InputIterator last, const std::string& sep = " ")
{
  std::stringstream result("");
  if (first != last) result << *first++;
  for (; first != last; ++first) result << sep << *first;
  return result.str();
}

/**
 * \brief Joins all elements of a container into one string.
 * \param cont Container whose elements will be joined.
 * \param sep String that will separate the joined elements.
 * \return Concatenated string.
 */
template<class Container> inline std::string
join(const Container& cont, const std::string& sep = " ")
{ return join(cont.begin(), cont.end(), sep); }


/**
 * Reference to a single field in a %SLHA structure.
 * This struct represents a reference to a single field in a %SLHA
 * structure, but which is independent of any concrete SLHA object.
 * That means that only the keys and the index of the SLHA, SLHABlock,
 * and SLHALine containers of the corresponding field are stored. One
 * of the main purposes of this struct is the conversion to string and
 * vice versa in a way that the string representation of a %SLHAKey
 * can be used as a single field in a %SLHA structure. For example,
 * the string representation of a %SLHAKey that refers to the entry in
 * the first row and third column of the RVHMIX matrix is
 * \c "RVHMIX;1,3;2". Further examples are \c "1000022;DECAY;2" which
 * refers to the total decay width of the lightest neutralino or
 * \c "1000022;(any),2,11,24;0" which refers to the branching ratio of
 * the decay of the lightest neutralino into an electron and a W
 * boson.
 */
struct SLHAKey
{
  /** Name of the SLHABlock that contains the field. */
  std::string block;

  /** First field(s) of the SLHALine that contains the field. */
  std::vector<std::string> line;

  /** Index of the field in the SLHALine. */
  std::size_t field;

  /**
   * \brief Constructs a %SLHAKey from explicit key values.
   * \param _block Name of the SLHABlock that contains the field.
   * \param _line First field(s) of the SLHALine that contains the
   *   field.
   * \param _field Index of the field in the SLHALine.
   */
  SLHAKey(
    const std::string& _block,
    const std::vector<std::string>& _line,
    std::size_t _field)
      : block(_block),
        line(_line),
        field(_field) {}

  /**
   * \brief Constructs a %SLHAKey from a string.
   * \param keyString String from which the %SLHAKey is constructed.
   * \sa str()
   */
  SLHAKey(const std::string& keyString)
  { str(keyString); }

  /**
   * \brief Constructs a %SLHAKey from a string.
   * \param keyString String from which the %SLHAKey is constructed.
   * \sa str()
   */
  SLHAKey(const char* keyString)
  { str(keyString); }

  /**
   * \brief Converts a string to a %SLHAKey.
   * \param keyString String that represents a %SLHAKey.
   * \return Reference to \c *this.
   */
  SLHAKey&
  str(const std::string& keyString)
  {
    std::vector<std::string> keys = split_string(keyString, ";");
    if (keys.size() != 3)
    { throw std::invalid_argument("SLHAKey::str(\"" + keyString + "\");"); }

    block = keys[0];
    line  = split_string(keys[1], ",");
    field = to_<std::size_t>(keys[2]);
    return *this;
  }

  /**
   * \brief Converts a %SLHAKey into its string representation.
   * \return String that represents the %SLHAKey.
   */
  std::string
  str() const
  {
    std::stringstream result("");
    result << block << ";" << join(line, ",") << ";" << field;
    return result.str();
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
 * Container of strings that represents a line in a %SLHA structure.
 * This class is a container of strings that represents a line in a
 * %SLHA structure. The elements of a %SLHALine are the so called
 * fields of an ordinary %SLHA line, which are its
 * whitespace-separated substrings and the comment. For example, if a
 * %SLHALine is constructed from the line <tt>" 1 2 0.123 # a comment
 * "</tt> its elements would be \c "1", \c "2", \c "0.123", and
 * \c "# a comment". Array-style access to the elements with integer
 * indices is provided by the operator[]() and at() functions.
 *
 * In addition to storing the fields of a %SLHA line, a %SLHALine also
 * stores its formatting (the exact position of the fields in the
 * line). A formatted representation of a %SLHALine can be produced
 * with str() const. The reformat() function clears the previous
 * formatting and indents all elements with a appropriate number of
 * spaces.
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
  typedef impl_type::pointer                pointer;
  typedef impl_type::const_pointer          const_pointer;
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
   * \brief Assigns content to the %SLHALine based on a string.
   * \param line String whose fields are used as content of the
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
   * last element is a comment, \p field is only appended to it and
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
   * \brief Reformats the string representation of the %SLHALine.
   * \return Reference to \c *this.
   */
  SLHALine&
  reformat()
  {
    if (empty()) return *this;

    std::stringstream line_format("");
    int arg = 0, pos = 0;
    const_iterator field = begin();

    if (is_block_specifier(*field))
    {
      line_format << " %|" << pos << "t|%" << ++arg << "%";
      pos += field->length();

      if (field+1 != end())
      {
        line_format << " %|" << ++pos << "t|%" << ++arg << "%";
        pos += (++field)->length();
      }
    }
    else if ((*field)[0] == '#')
    {
      line_format << " %|" << pos << "t|%" << ++arg << "%";
      pos += field->length();
    }
    else
    {
      line_format << " %|" << ++pos << "t|%" << ++arg << "%";
      pos += field->length();
    }

    while (++field != end())
    {
      // Compute the number of spaces required for proper indentation.
      int dist = 3 - ((pos - 1) % 4);
      pos += dist > 1 ? dist : dist + 4;

      line_format << " %|" << pos << "t|%" << ++arg << "%";
      pos += field->length();
    }

    lineFormat_ = line_format.str().substr(1);
    return *this;
  }

  /**
   * \brief Assigns content to the %SLHALine based on a string.
   * \param line String whose fields are used as content of the
   *   %SLHALine.
   * \return Reference to \c *this.
   *
   * This function parses \p line and sets the found fields as content
   * of the %SLHALine. If \p line contains newlines, everything after
   * the first newline is ignored.
   *
   * The exact formatting of \p line is stored internally and can be
   * reproduced with str() const.
   */
  SLHALine&
  str(const std::string& line)
  {
    clear();
    const std::string
      line_tr = boost::trim_right_copy(line.substr(0, line.find('\n')));
    if (line_tr.empty()) return *this;

    const std::size_t
      comment_pos = std::min(line_tr.find('#'), line_tr.length());
    const std::string data    = line_tr.substr(0, comment_pos);
    const std::string comment = line_tr.substr(comment_pos);

    std::stringstream line_format("");
    int argument = 0;

    const std::string delimiters = " \t\v\f\r";
    std::size_t pos1 = data.find_first_not_of(delimiters, 0);
    std::size_t pos2 = data.find_first_of(delimiters, pos1);

    while (pos1 != std::string::npos)
    {
      line_format << " %|" << pos1 << "t|%" << ++argument << "%";

      impl_.push_back(data.substr(pos1, pos2 - pos1));
      pos1 = data.find_first_not_of(delimiters, pos2);
      pos2 = data.find_first_of(delimiters, pos1);
    }

    if (!comment.empty())
    {
      line_format << " %|" << comment_pos << "t|%" << ++argument << "%";
      impl_.push_back(comment);
    }

    lineFormat_ = line_format.str().substr(1);
    return *this;
  }

  /** Returns a formatted string representation of the %SLHALine. */
  std::string
  str() const
  {
    boost::format formatter(lineFormat_);
    for (const_iterator field = begin(); field != end(); ++field)
    { formatter % *field; }
    return formatter.str();
  }

  // element access
  /**
   * \brief Subscript access to the strings contained in the
   *   %SLHALine.
   * \param n Index of the string which should be accessed.
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
   * \param n Index of the string which should be accessed.
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
   * \param n Index of the string which should be accessed.
   * \return Read/write reference to the accessed string.
   * \throw std::out_of_range If \p n is an invalid index.
   */
  reference
  at(size_type n)
  { return impl_.at(n); }

  /**
   * \brief Provides access to the strings contained in the %SLHALine.
   * \param n Index of the string which should be accessed.
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

  // introspection
  /**
   * Returns true if the %SLHALine begins with \c "BLOCK" or
   * \c "DECAY" followed by a block name. Comparison is done
   * case-insensitive.
   */
  bool
  is_block_def() const
  {
    if (size() < 2) return false;

    const_iterator field = begin();
    return (is_block_specifier(*field) && ((*++field)[0] != '#'));
  }

  /** Returns true if the %SLHALine begins with \c "#". */
  bool
  is_comment_line() const
  { return !empty() && (front()[0] == '#'); }

  /**
   * Returns true if the %SLHALine is not empty and if neither
   * is_block_def() nor is_comment_line() returns true.
   */
  bool
  is_data_line() const
  { return !empty() && !is_block_def() && !is_comment_line(); }

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
    size_type data_size = size();
    if ((data_size > 0) && (back()[0] == '#')) --data_size;
    return data_size;
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

  /**
   * Comments the %SLHALine. This function prefixes the %SLHALine
   * with a \c "#" and packetizes all its elements into one.
   */
  void
  comment()
  {
    if (empty()) return;
    str("#" + str());
  }

  /**
   * Uncomments the %SLHALine. This function removes the first
   * character of the %SLHALine if it is a \c "#" and splits the
   * former comment into the corresponding number of fields.
   */
  void
  uncomment()
  {
    if (empty()) return;
    if (front()[0] == '#')
    {
      front().erase(0, 1);
      str(str());
    }
  }

private:
  inline bool
  is_block_specifier(const value_type& field) const
  {
    const value_type field_upper = boost::to_upper_copy(field);
    return (field_upper == "BLOCK") || (field_upper == "DECAY");
  }

private:
  impl_type impl_;
  std::string lineFormat_;
};


/**
 * Container of \SLHALines that resembles a block in a %SLHA
 * structure. This class is a named container of \SLHALines that
 * resembles a block in a %SLHA structure. Unlike to a block in a
 * %SLHA structure, a %SLHABlock can contain any number of block
 * definitions or it can be completely empty.
 *
 * Access to the elements of the %SLHABlock is provided by the
 * operator[]() and at() functions. These take one or more strings
 * resp. ints as argument(s) and compare them against the first
 * strings of the contained \SLHALines (the ints are converted to
 * strings before comparison). The first SLHALine that matches the
 * provided arguments is then returned, or if no matching SLHALine is
 * found, an empty SLHALine is appended to the %SLHABlock
 * (operator[]()) or \c std::out_of_range is thrown (at()). The
 * special argument \c "(any)" will be considered equal to all strings
 * in the \SLHALines. For example, <tt>at("(any)", "2")</tt> returns
 * the first SLHALine whose second element is \c "2".
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
  typedef impl_type::pointer                pointer;
  typedef impl_type::const_pointer          const_pointer;
  typedef impl_type::difference_type        difference_type;
  typedef impl_type::size_type              size_type;

  /**
   * \brief Constructs an empty %SLHABlock.
   * \param name Name of the %SLHABlock.
   */
  explicit
  SLHABlock(const std::string& name = "") : name_(name) {}

  // NOTE: The compiler-generated copy constructor and assignment
  //   operator for this class are just fine, so we don't need to
  //   write our own.

  /**
   * \brief Sets the name of the %SLHABlock.
   * \param newName New name of the %SLHABlock.
   *
   * Notice that this function only changes a property of the
   * %SLHABlock. No contained SLHALine (in particular no block
   * definition) is changed.
   */
  void
  name(const std::string& newName)
  { name_ = newName; }

  /** Returns the name of the %SLHABlock. */
  const std::string&
  name() const
  { return name_; }

  /**
   * \brief Assigns content from input stream to the %SLHABlock.
   * \param is Input stream to read content from.
   * \return Reference to \c *this.
   *
   * This functions clears the content of the %SLHABlock, reads
   * non-empty lines from \p is, transforms them into \SLHALines
   * and adds them to the end of the %SLHABlock.
   */
  SLHABlock&
  read(std::istream& is)
  {
    std::string line_str;
    value_type line;
    bool nameless = true;

    clear();
    while (std::getline(is, line_str))
    {
      if (boost::all(line_str, boost::is_space())) continue;

      line = line_str;
      if (nameless && line.is_block_def())
      {
        name(line[1]);
        nameless = false;
      }
      push_back(line);
    }
    return *this;
  }

  /**
   * \brief Assigns content to the %SLHABlock based on a string.
   * \param block String that is used as content for the %SLHABlock.
   * \return Reference to \c *this.
   *
   * This functions clears the content of the %SLHABlock and adds
   * every non-empty line found in \p block as SLHALine to the end of
   * the %SLHABlock.
   */
  SLHABlock&
  str(const std::string& block)
  {
    std::stringstream ss(block);
    return read(ss);
  }

  /** Returns a string representation of the %SLHABlock. */
  std::string
  str() const
  {
    std::stringstream ss("");
    ss << *this;
    return ss.str();
  }

  // element access
  /**
   * \brief Locates a SLHALine in the %SLHABlock.
   * \param keys First strings of the SLHALine to be located.
   * \return Read/write reference to sought-after SLHALine.
   *
   * This function takes a key (which is a vector of strings) and
   * locates the SLHALine whose first strings are equal to the strings
   * in \p keys. If no such SLHALine exists, this function creates an
   * empty SLHALine at the end of the %SLHABlock and returns a
   * reference to it.
   */
  reference
  operator[](const key_type& keys)
  {
    iterator line = find(keys);
    if (line == end())
    {
      push_back(value_type());
      return back();
    }
    return *line;
  }

  /**
   * \brief Locates a SLHALine in the %SLHABlock.
   * \param keys Integers that are used to locate the SLHALine.
   * \return Read/write reference to sought-after SLHALine.
   *
   * This function takes a key (which is a vector of ints) and locates
   * the SLHALine whose first strings are equal to the to strings
   * converted ints in \p keys. If no such SLHALine exists, this
   * function creates an empty SLHALine at the end of the %SLHABlock
   * and returns a reference to it.
   */
  reference
  operator[](const std::vector<int>& keys)
  { return (*this)[cont_to_string_vector(keys)]; }

  /**
   * \brief Locates a SLHALine in the %SLHABlock.
   * \param key String that is used to locate the SLHALine.
   * \return Read/write reference to sought-after SLHALine.
   *
   * This function locates the SLHALine whose first string is equal to
   * \p key. If no such SLHALine exists, this function creates an
   * empty SLHALine at the end of the %SLHABlock and returns a
   * reference to it.
   */
  reference
  operator[](const std::string& key)
  { return (*this)[std::vector<std::string>(1, key)]; }

  /**
   * \brief Locates a SLHALine in the %SLHABlock.
   * \param key Integer that is used to locate the SLHALine.
   * \return Read/write reference to sought-after SLHALine.
   *
   * This function locates the SLHALine whose first string is equal to
   * the to string converted \p key. If no such SLHALine exists, this
   * function creates an empty SLHALine at the end of the %SLHABlock
   * and returns a reference to it.
   */
  reference
  operator[](int key)
  { return (*this)[std::vector<std::string>(1, to_string(key))]; }

  /**
   * \brief Locates a SLHALine in the %SLHABlock.
   * \param keys First strings of the SLHALine to be located.
   * \return Read/write reference to sought-after SLHALine.
   * \throw std::out_of_range If \p keys does not match any SLHALine.
   *
   * This function takes a key (which is a vector of strings) and
   * locates the SLHALine whose first strings are equal to the strings
   * in \p keys. If no such SLHALine exists, \c std::out_of_range is
   * thrown.
   */
  reference
  at(const key_type& keys)
  {
    iterator line = find(keys);
    if (line == end())
    { throw std::out_of_range("SLHABlock::at(\"" + join(keys) + "\")"); }
    return *line;
  }

  /**
   * \brief Locates a SLHALine in the %SLHABlock.
   * \param keys First strings of the SLHALine to be located.
   * \return Read-only (constant) reference to sought-after SLHALine.
   * \throw std::out_of_range If \p keys does not match any SLHALine.
   *
   * This function takes a key (which is a vector of strings) and
   * locates the SLHALine whose first strings are equal to the strings
   * in \p keys. If no such SLHALine exists, \c std::out_of_range is
   * thrown.
   */
  const_reference
  at(const key_type& keys) const
  {
    const_iterator line = find(keys);
    if (line == end())
    { throw std::out_of_range("SLHABlock::at(\"" + join(keys) + "\")"); }
    return *line;
  }

  /**
   * \brief Locates a SLHALine in the %SLHABlock.
   * \param keys Integers that are used to locate the SLHALine.
   * \return Read/write reference to sought-after SLHALine.
   * \throw std::out_of_range If \p keys does not match any SLHALine.
   *
   * This function takes a key (which is a vector of ints) and locates
   * the SLHALine whose first strings are equal to the to strings
   * converted ints in \p keys. If no such SLHALine exists,
   * \c std::out_of_range is thrown.
   */
  reference
  at(const std::vector<int>& keys)
  { return at(cont_to_string_vector(keys)); }

  /**
   * \brief Locates a SLHALine in the %SLHABlock.
   * \param keys Integers that are used to locate the SLHALine.
   * \return Read-only (constant) reference to sought-after SLHALine.
   * \throw std::out_of_range If \p keys does not match any SLHALine.
   *
   * This function takes a key (which is a vector of ints) and locates
   * the SLHALine whose first strings are equal to the to strings
   * converted ints in \p keys. If no such SLHALine exists,
   * \c std::out_of_range is thrown.
   */
  const_reference
  at(const std::vector<int>& keys) const
  { return at(cont_to_string_vector(keys)); }

  /**
   * \brief Locates a SLHALine in the %SLHABlock.
   * \param s0, s1, s2, s3 First strings of the SLHALine to be
   *   located.
   * \return Read/write reference to sought-after SLHALine.
   * \throw std::out_of_range If provided strings do not match any
   *   SLHALine.
   *
   * This function takes up to four strings and locates the SLHALine
   * whose first strings are equal to all provided non-empty strings.
   * If no such SLHALine exists, \c std::out_of_range is thrown.
   */
  reference
  at(const std::string& s0,      const std::string& s1 = "",
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

  /**
   * \brief Locates a SLHALine in the %SLHABlock.
   * \param s0, s1, s2, s3 First strings of the SLHALine to be
   *   located.
   * \return Read-only (constant) reference to sought-after SLHALine.
   * \throw std::out_of_range If provided strings do not match any
   *   SLHALine.
   *
   * This function takes up to four strings and locates the SLHALine
   * whose first strings are equal to all provided non-empty strings.
   * If no such SLHALine exists, \c std::out_of_range is thrown.
   */
  const_reference
  at(const std::string& s0,      const std::string& s1 = "",
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

  /**
   * \brief Locates a SLHALine in the %SLHABlock.
   * \param i0, i1, i2, i3 Integers that are used to locate the
   *   SLHALine.
   * \return Read/write reference to sought-after SLHALine.
   * \throw std::out_of_range If provided ints do not match any
   *   SLHALine.
   *
   * This function takes up to four ints and locates the SLHALine
   * whose first strings are equal to all provided to string converted
   * ints. If no such SLHALine exists, \c std::out_of_range is thrown.
   */
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

  /**
   * \brief Locates a SLHALine in the %SLHABlock.
   * \param i0, i1, i2, i3 Integers that are used to locate the
   *   SLHALine.
   * \return Read-only (constant) reference to sought-after SLHALine.
   * \throw std::out_of_range If provided ints do not match any
   *   SLHALine.
   *
   * This function takes up to four ints and locates the SLHALine
   * whose first strings are equal to all provided to string converted
   * ints. If no such SLHALine exists, \c std::out_of_range is thrown.
   */
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
   * Returns a read/write reference to the first element of the
   * %SLHABlock.
   */
  reference
  front()
  { return impl_.front(); }

  /**
   * Returns a read-only (constant) reference to the first element of
   * the %SLHABlock.
   */
  const_reference
  front() const
  { return impl_.front(); }

  /**
   * Returns a read/write reference to the last element of the
   * %SLHABlock.
   */
  reference
  back()
  { return impl_.back(); }

  /**
   * Returns a read-only (constant) reference to the last element of
   * the %SLHABlock.
   */
  const_reference
  back() const
  { return impl_.back(); }

  // iterators
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

  // operations
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

    iterator line = begin();
    for (; line != end(); ++line)
    {
      if (keys.size() > line->size()) continue;
      if (std::equal(keys.begin(), keys.end(), line->begin(), index_iequal))
      { return line; }
    }
    return line;
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

    const_iterator line = begin();
    for (; line != end(); ++line)
    {
      if (keys.size() > line->size()) continue;
      if (std::equal(keys.begin(), keys.end(), line->begin(), index_iequal))
      { return line; }
    }
    return line;
  }

  // introspection
  /**
   * \brief Counts all \SLHALines that match a given key.
   * \param keys First strings of the \SLHALines that will be counted.
   * \return Number of lines whose first strings equal \p keys.
   */
  size_type
  count(const key_type& keys) const
  {
    if (keys.empty()) return 0;

    size_type count = 0;
    for (const_iterator line = begin(); line != end(); ++line)
    {
      if (keys.size() > line->size()) continue;
      if (std::equal(keys.begin(), keys.end(), line->begin(), index_iequal))
      { ++count; }
    }
    return count;
  }

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

  /**
   * \brief Adds a SLHALine to the end of the %SLHABlock.
   * \param line String that is used to construct the SLHALine that
   *   will be added.
   *
   * This function creates an element at the end of the %SLHABlock and
   * assigns the SLHALine that is constructed from \p line to it.
   */
  void
  push_back(const std::string& line)
  { impl_.push_back(value_type(line)); }

  /**
   * Removes the last element. This function shrinks the size() of the
   * %SLHABlock by one.
   */
  void
  pop_back()
  { impl_.pop_back(); }

  /**
   * \brief Inserts a SLHALine before given \p position.
   * \param position Iterator into the %SLHABlock.
   * \param line SLHALine to be inserted.
   * \return Iterator pointing to the inserted element.
   *
   * This function inserts a copy of \p line before the specified
   * \p position and thus enlarges the %SLHABlock by one.
   */
  iterator
  insert(iterator position, const value_type& line)
  { return impl_.insert(position, line); }

  /**
   * \brief Inserts a range into the %SLHABlock.
   * \param position Iterator into the %SLHABlock.
   * \param first Input iterator.
   * \param last Input iterator.
   *
   * This function inserts copies of the \SLHALines in the range
   * [\p first, \p last) into the %SLHABlock before the specified
   * \p position and thus enlarges the %SLHABlock accordingly.
   */
  template<class InputIterator> void
  insert(iterator position, InputIterator first, InputIterator last)
  { impl_.insert(position, first, last); }

  /**
   * \brief Removes element at given \p position.
   * \param position Iterator pointing to the element to be erased.
   * \return Iterator pointing to the next element (or end()).
   *
   * This function erases the element at the given \p position and
   * thus shortens the %SLHABlock by one.
   */
  iterator
  erase(iterator position)
  { return impl_.erase(position); }

 /**
   * \brief Removes a range of elements.
   * \param first Iterator pointing to the first element to be erased.
   * \param last Iterator pointing to one past the last element to be
   *   erased.
   * \return Iterator pointing to the element pointed to by \p last
   *   prior to erasing (or end()).
   *
   * This function erases the elements in the range [\p first,
   * \p last) and shortens the %SLHABlock accordingly.
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

  /**
   * \brief Comments all \SLHALines in the %SLHABlock.
   * \sa SLHALine::comment()
   */
  void
  comment()
  { std::for_each(begin(), end(), std::mem_fun_ref(&value_type::comment)); }

  /**
   * \brief Uncomments all \SLHALines in the %SLHABlock.
   * \sa SLHALine::uncomment()
   */
  void
  uncomment()
  { std::for_each(begin(), end(), std::mem_fun_ref(&value_type::uncomment)); }

private:
  static bool
  index_iequal(const std::string& a, const std::string& b)
  {
    if ("(any)" == a) return true;
    return boost::iequals(a, b);
  }

private:
  std::string name_;
  impl_type impl_;
  static const int nind = INT_MIN;
};


/**
 * Container of \SLHABlocks that resembles a complete %SLHA structure.
 * This class is a container of \SLHABlocks that resembles a complete
 * %SLHA structure. The elements of the SLHA objects can be accessed
 * via their names with the operator[]() and at() functions and access
 * to single fields is provided by the field() functions. To fill this
 * container, the functions read() or str() can be used which read
 * data from an input stream or a string, respectively.
 */
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
  typedef impl_type::pointer                pointer;
  typedef impl_type::const_pointer          const_pointer;
  typedef impl_type::difference_type        difference_type;
  typedef impl_type::size_type              size_type;

  /** Constructs an empty %SLHA container. */
  SLHA() {}

  // NOTE: The compiler-generated copy constructor and assignment
  //   operator for this class are just fine, so we don't need to
  //   write our own.

  /**
   * \brief Constructs %SLHA container with content from input stream.
   * \param is Input stream to read content from.
   * \sa read()
   */
  explicit
  SLHA(std::istream& is)
  { read(is); }

  /**
   * \brief Accesses a single field in the %SLHA container.
   * \param key SLHAKey that refers to the field that should be
   *   accessed.
   * \return Read/write reference to the field referred to by \p key.
   * \throw std::out_of_range If \p key refers to a non-existing
   *   field.
   */
  SLHALine::reference
  field(const SLHAKey& key)
  { return at(key.block).at(key.line).at(key.field); }

  /**
   * \brief Accesses a single field in the %SLHA container.
   * \param key SLHAKey that refers to the field that should be
   *   accessed.
   * \return Read-only (constant) reference to the field referred to
   *   by \p key.
   * \throw std::out_of_range If \p key refers to a non-existing
   *   field.
   */
  SLHALine::const_reference
  field(const SLHAKey& key) const
  { return at(key.block).at(key.line).at(key.field); }

  /**
   * \brief Assigns content from input stream to the %SLHA container.
   * \param is Input stream to read content from.
   * \returns Reference to \c *this.
   *
   * This function reads non-empty lines from \p is, transforms them
   * into \SLHALines, which are collected into the corresponding
   * \SLHABlocks that are added to the %SLHA container.
   */
  SLHA&
  read(std::istream& is)
  {
    std::string line_str;
    SLHALine line;
    pointer block = &(*this)[""];

    while (std::getline(is, line_str))
    {
      if (boost::all(line_str, boost::is_space())) continue;

      line = line_str;
      if (line.is_block_def()) block = &(*this)[line[1]];
      block->push_back(line);
    }

    erase_if_empty("");
    return *this;
  }

  /**
   * \brief Assigns content to the %SLHA container based on a string.
   * \param slhaString String that is used as content for the %SLHA
   *   container.
   * \returns Reference to \c *this.
   */
  SLHA&
  str(const std::string& slhaString)
  {
    std::stringstream ss(slhaString);
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
   * \brief Locates a SLHABlock in the %SLHA container.
   * \param blockName Name of the SLHABlock to be located.
   * \return Read/write reference to sought-after SLHABlock.
   *
   * This function locates the first SLHABlock whose name equals
   * \p blockName. If no such SLHABlock is present, an empty SLHABlock
   * with this name is added to the end of the %SLHA container and a
   * reference to it is then returned.
   */
  reference
  operator[](const key_type& blockName)
  {
    iterator block = find(blockName);
    if (block == end())
    {
      push_back(value_type(blockName));
      return back();
    }
    return *block;
  }

  /**
   * \brief Locates a SLHABlock in the %SLHA container.
   * \param blockName Name of the SLHABlock to be located.
   * \return Read/write reference to sought-after SLHABlock.
   * \throw std::out_of_range If no SLHABlock with the name
   *   \p blockName exists.
   */
  reference
  at(const key_type& blockName)
  {
    iterator block = find(blockName);
    if (block == end())
    { throw std::out_of_range("SLHA::at(\"" + blockName + "\")"); }
    return *block;
  }

  /**
   * \brief Locates a SLHABlock in the %SLHA container.
   * \param blockName Name of the SLHABlock to be located.
   * \return Read-only (constant) reference to sought-after SLHABlock.
   * \throw std::out_of_range If no SLHABlock with the name
   *   \p blockName exists.
   */
  const_reference
  at(const key_type& blockName) const
  {
    const_iterator block = find(blockName);
    if (block == end())
    { throw std::out_of_range("SLHA::at(\"" + blockName + "\")"); }
    return *block;
  }

  /**
   * Returns a read/write reference to the first element of the %SLHA
   * container.
   */
  reference
  front()
  { return impl_.front(); }

  /**
   * Returns a read-only (constant) reference to the first element of
   * the %SLHA container.
   */
  const_reference
  front() const
  { return impl_.front(); }

  /**
   * Returns a read/write reference to the last element of the %SLHA
   * container.
   */
  reference
  back()
  { return impl_.back(); }

  /**
   * Returns a read-only (constant) reference to the last element of
   * the %SLHA container.
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
  /**
   * \brief Tries to locate a SLHABlock in the %SLHA container.
   * \param blockName Name of the SLHABlock to be located.
   * \return Read/write iterator pointing to sought-after element, or
   *   end() if not found.
   *
   * This function takes a key and tries to locate the SLHABlock whose
   * name matches \p blockName (comparison is case-insensitive). If
   * successful the function returns a read/write iterator pointing to
   * the sought after SLHABlock. If unsuccessful it returns end().
   */
  iterator
  find(const key_type& blockName)
  { return std::find_if(begin(), end(), name_iequals(blockName)); }

  /**
   * \brief Tries to locate a SLHABlock in the %SLHA container.
   * \param blockName Name of the SLHABlock to be located.
   * \return Read-only (constant) iterator pointing to sought-after
   *   element, or end() const if not found.
   *
   * This function takes a key and tries to locate the SLHABlock whose
   * name matches \p blockName (comparison is case-insensitive). If
   * successful the function returns a read-only (constant) iterator
   * pointing to the sought after SLHABlock. If unsuccessful it
   * returns end() const.
   */
  const_iterator
  find(const key_type& blockName) const
  { return std::find_if(begin(), end(), name_iequals(blockName)); }

  // introspection
  /**
   * \brief Counts all \SLHABlocks with a given name.
   * \param blockName Name of the \SLHABlocks that will be counted.
   * \return Number of blocks whose name equals \p blockName.
   *
   * Notice that the comparison of \p blockName and the names of the
   * \SLHABlocks is case-insensitive.
   */
  size_type
  count(const key_type& blockName) const
  { return std::count_if(begin(), end(), name_iequals(blockName)); }

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
   * \brief Adds a SLHABlock to the end of the %SLHA container.
   * \param blockString String that is used to construct the SLHABlock
   *   that will be added.
   *
   * This function creates an element at the end of the %SLHA
   * container and assigns the SLHABlock that is constructed from
   * \p blockString to it.
   */
  void
  push_back(const std::string& blockString)
  {
    value_type block;
    block.str(blockString);
    impl_.push_back(block);
  }

  /**
   * Removes the last element. This function shrinks the size() of the
   * %SLHA container by one.
   */
  void
  pop_back()
  { impl_.pop_back(); }

  /**
   * \brief Inserts a SLHABlock before given \p position.
   * \param position Iterator into the %SLHA container.
   * \param block SLHABlock to be inserted.
   * \return Iterator pointing to the inserted element.
   *
   * This function inserts a copy of \p block before the specified
   * \p position and thus enlarges the %SLHA container by one.
   */
  iterator
  insert(iterator position, const value_type& block)
  { return impl_.insert(position, block); }

  /**
   * \brief Inserts a range into the %SLHA container.
   * \param position Iterator into the %SLHA container.
   * \param first Input iterator.
   * \param last Input iterator.
   *
   * This function inserts copies of the \SLHABlocks in the range
   * [\p first, \p last) into the %SLHA container before the specified
   * \p position and thus enlarges the %SLHA container accordingly.
   */
  template<class InputIterator> void
  insert(iterator position, InputIterator first, InputIterator last)
  { impl_.insert(position, first, last); }

  /**
   * \brief Removes element at given \p position.
   * \param position Iterator pointing to the element to be erased.
   * \return Iterator pointing to the next element (or end()).
   *
   * This function erases the element at the given \p position and
   * thus shortens the %SLHA container by one.
   */
  iterator
  erase(iterator position)
  { return impl_.erase(position); }

  /**
   * \brief Removes a range of elements.
   * \param first Iterator pointing to the first element to be erased.
   * \param last Iterator pointing to one past the last element to be
   *   erased.
   * \return Iterator pointing to the element pointed to by \p last
   *   prior to erasing (or end()).
   *
   * This function erases the elements in the range [\p first,
   * \p last) and shortens the %SLHA container accordingly.
   */
  iterator
  erase(iterator first, iterator last)
  { return impl_.erase(first, last); }

  /**
   * \brief Tries to erase a SLHABlock in the %SLHA container.
   * \param blockName Name of the SLHABlock to be erased.
   * \return Iterator pointing to the next element (or end()).
   *
   * This function takes a key and tries to erase the SLHABlock whose
   * name matches \p blockName (comparison is case-insensitive). If
   * the %SLHA container contains such SLHABlock, the function returns
   * an iterator pointing to the next element (or end()). If no such
   * SLHABlock exists, end() is returned.
   */
  iterator
  erase(const key_type& blockName)
  {
    iterator block = find(blockName);
    if (block != end()) return erase(block);
    return block;
  }

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
  struct name_iequals : public std::unary_function<value_type, bool>
  {
    explicit
    name_iequals(const key_type& blockName) : name_(blockName) {}

    bool
    operator()(const value_type& block) const
    { return boost::iequals(name_, block.name()); }

  private:
    key_type name_;
  };

  inline iterator
  erase_if_empty(const key_type& blockName)
  {
    iterator block = find(blockName);
    if (block != end() && block->empty()) return erase(block);
    return block;
  }

private:
  impl_type impl_;
};


// stream operators
inline std::istream&
operator>>(std::istream& is, SLHABlock& block)
{
  block.read(is);
  return is;
}

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
  std::copy(block.begin(), block.end(),
            std::ostream_iterator<SLHABlock::value_type>(os, "\n"));
  return os;
}

inline std::ostream&
operator<<(std::ostream& os, const SLHA& slha)
{
  std::copy(slha.begin(), slha.end(),
            std::ostream_iterator<SLHA::value_type>(os));
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
