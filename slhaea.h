// SLHAea - another SUSY Les Houches Accord input/output library
// Copyright © 2009-2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SLHAEA_H
#define SLHAEA_H

#include <algorithm>
#include <cstddef>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
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
 * This function is a wrapper for
 * \c boost::lexical_cast<Target>().
 */
template<class Target, class Source> inline Target
to(const Source& arg)
{ return boost::lexical_cast<Target>(arg); }

/**
 * \brief Converts an object of type \c Source to a string.
 * \param arg Object that will be converted.
 * \return Result of the conversion of \p arg to \c std::string.
 *
 * This function is a wrapper for
 * \c boost::lexical_cast<std::string>().
 */
template<class Source> inline std::string
to_string(const Source& arg)
{ return boost::lexical_cast<std::string>(arg); }

/**
 * \brief Converts an object of type \c Source to a string.
 * \param arg Object that will be converted.
 * \param precision Precision of float values that are written in
 *   scientific notation.
 * \return Result of the conversion of \p arg to \c std::string.
 *
 * This function is equivalent to \c to_string() except that all
 * floating-point numbers are written in scientific notation with the
 * given precision.
 */
template<class Source> inline std::string
to_string(const Source& arg, int precision)
{
  std::stringstream output("");
  output << std::setprecision(precision) << std::scientific << arg;
  return output.str();
}


// forward declarations
class Line;
class Block;
class Coll;
struct Key;

inline std::ostream& operator<<(std::ostream& os, const Line& line);
inline std::ostream& operator<<(std::ostream& os, const Block& block);
inline std::ostream& operator<<(std::ostream& os, const Coll& coll);
inline std::ostream& operator<<(std::ostream& os, const Key& key);


/**
 * Container of strings that represents a line in a SLHA structure.
 * This class is a container of strings that represents a line in a
 * SLHA structure. The elements of a %Line are the so called fields of
 * an ordinary SLHA line, which are its whitespace-separated
 * substrings and the comment. For example, if a %Line is constructed
 * from the string <tt>" 1 2 0.123 # a comment"</tt> its elements
 * would be \c "1", \c "2", \c "0.123", and \c "# a comment".
 * Array-style access to the elements with integer indices is provided
 * by the operator[]() and at() functions.
 *
 * In addition to storing the fields of a SLHA line, a %Line also
 * stores its formatting (the exact position of the fields in the
 * line). A formatted representation of a %Line can be produced with
 * str() const. The reformat() function clears the previous formatting
 * and indents all elements with a appropriate number of spaces.
 */
class Line
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

  /** Constructs an empty %Line. */
  Line() : impl_(0), bounds_(0), format_("") {}

  // NOTE: The compiler-generated copy constructor and assignment
  //   operator for this class are just fine, so we don't need to
  //   write our own.

  /**
   * \brief Constructs a %Line from a string.
   * \param line String whose fields are used as content of the %Line.
   * \sa str()
   */
  Line(const std::string& line)
  { str(line); }

  /**
   * \brief Assigns content to the %Line based on a string.
   * \param line String whose fields are used as content of the %Line.
   * \return Reference to \c *this.
   *
   * This function is an alias for str().
   */
  Line&
  operator=(const std::string& line)
  {
    str(line);
    return *this;
  }

  /**
   * \brief Appends a string to the end of the %Line.
   * \param arg String that is appended to the %Line.
   * \return Reference to \c *this.
   *
   * This function is an alias for append().
   */
  Line&
  operator+=(const std::string& arg)
  {
    append(arg);
    return *this;
  }

  /**
   * \brief Inserts an element at the end of the %Line.
   * \param field Element that is inserted at the end of the %Line.
   * \return Reference to \c *this.
   *
   * This function inserts an element at the end of the %Line. If the
   * the %Line contains a comment, \p field is only appended to the
   * last element and thus size() remains unchanged.
   */
  template<class T> Line&
  operator<<(const T& field)
  {
    std::string field_str = boost::lexical_cast<std::string>(field);
    boost::trim_right(field_str);
    if (field_str.empty()) return *this;

    if (contains_comment())
    { back() += field_str; }
    else
    {
      boost::trim_left(field_str);
      impl_.push_back(field_str);
      reformat();
    }
    return *this;
  }

  /**
   * \brief Appends a string to the end of the %Line.
   * \param arg String that is appended to the %Line.
   * \return Reference to \c *this.
   *
   * This functions appends \p arg to the output of str() const and
   * uses this temporary string as input for str(). Based on the
   * temporary string, size() is increased or remains unchanged.
   */
  Line&
  append(const std::string& arg)
  {
    str(str() + arg);
    return *this;
  }

  /**
   * \brief Assigns content to the %Line based on a string.
   * \param line String whose fields are used as content of the %Line.
   * \return Reference to \c *this.
   *
   * This function parses \p line and sets the found fields as content
   * of the %Line. If \p line contains newlines, everything after the
   * first newline is ignored.
   *
   * The exact formatting of \p line is stored internally and can be
   * reproduced with str() const.
   */
  Line&
  str(const std::string& line)
  {
    clear();
    const std::string
      trimmed_line = boost::trim_right_copy(line.substr(0, line.find('\n')));
    if (trimmed_line.empty()) return *this;

    const std::size_t comment_pos = trimmed_line.find('#');
    const std::string data = trimmed_line.substr(0, comment_pos);

    static const std::string delimiters = " \t\v\f\r";
    std::size_t pos1 = data.find_first_not_of(delimiters, 0);
    std::size_t pos2 = data.find_first_of(delimiters, pos1);

    while (pos1 != std::string::npos)
    {
      impl_.push_back(data.substr(pos1, pos2 - pos1));
      bounds_.push_back(std::make_pair(pos1, pos2));

      pos1 = data.find_first_not_of(delimiters, pos2);
      pos2 = data.find_first_of(delimiters, pos1);
    }

    if (comment_pos != std::string::npos)
    {
      impl_.push_back(trimmed_line.substr(comment_pos));
      bounds_.push_back(std::make_pair(comment_pos, trimmed_line.length()));
    }
    return *this;
  }

  /** Returns a formatted string representation of the %Line. */
  std::string
  str() const
  {
    if (format_.empty()) build_format_str();

    boost::format formatter(format_);
    for (const_iterator field = begin(); field != end(); ++field)
    { formatter % *field; }
    return formatter.str();
  }

  // element access
  /**
   * \brief Subscript access to the strings contained in the %Line.
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
   * \brief Subscript access to the strings contained in the %Line.
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
   * \brief Provides access to the strings contained in the %Line.
   * \param n Index of the string which should be accessed.
   * \return Read/write reference to the accessed string.
   * \throw std::out_of_range If \p n is an invalid index.
   */
  reference
  at(size_type n)
  { return impl_.at(n); }

  /**
   * \brief Provides access to the strings contained in the %Line.
   * \param n Index of the string which should be accessed.
   * \return Read-only (constant) reference to the accessed string.
   * \throw std::out_of_range If \p n is an invalid index.
   */
  const_reference
  at(size_type n) const
  { return impl_.at(n); }

  /**
   * Returns a read/write reference to the first element of the %Line.
   */
  reference
  front()
  { return impl_.front(); }

  /**
   * Returns a read-only (constant) reference to the first element of
   * the %Line.
   */
  const_reference
  front() const
  { return impl_.front(); }

  /**
   * Returns a read/write reference to the last element of the %Line.
   */
  reference
  back()
  { return impl_.back(); }

  /**
   * Returns a read-only (constant) reference to the last element of
   * the %Line.
   */
  const_reference
  back() const
  { return impl_.back(); }

  // iterators
  /**
   * Returns a read/write iterator that points to the first element in
   * the %Line. Iteration is done in ordinary element order.
   */
  iterator
  begin()
  { return impl_.begin(); }

  /**
   * Returns a read-only (constant) iterator that points to the first
   * element in the %Line. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  begin() const
  { return impl_.begin(); }

  /**
   * Returns a read-only (constant) iterator that points to the first
   * element in the %Line. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  cbegin() const
  { return impl_.begin(); }

  /**
   * Returns a read/write iterator that points one past the last
   * element in the %Line. Iteration is done in ordinary element
   * order.
   */
  iterator
  end()
  { return impl_.end(); }

  /**
   * Returns a read-only (constant) iterator that points one past the
   * last element in the %Line. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  end() const
  { return impl_.end(); }

  /**
   * Returns a read-only (constant) iterator that points one past the
   * last element in the %Line. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  cend() const
  { return impl_.end(); }

  /**
   * Returns a read/write reverse iterator that points to the last
   * element in the %Line. Iteration is done in reverse element order.
   */
  reverse_iterator
  rbegin()
  { return impl_.rbegin(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * the last element in the %Line. Iteration is done in reverse
   * element order.
   */
  const_reverse_iterator
  rbegin() const
  { return impl_.rbegin(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * the last element in the %Line. Iteration is done in reverse
   * element order.
   */
  const_reverse_iterator
  crbegin() const
  { return impl_.rbegin(); }

  /**
   * Returns a read/write reverse iterator that points to one before
   * the first element in the %Line. Iteration is done in reverse
   * element order.
   */
  reverse_iterator
  rend()
  { return impl_.rend(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * one before the first element in the %Line. Iteration is done in
   * reverse element order.
   */
  const_reverse_iterator
  rend() const
  { return impl_.rend(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * one before the first element in the %Line. Iteration is done in
   * reverse element order.
   */
  const_reverse_iterator
  crend() const
  { return impl_.rend(); }

  // introspection
  /**
   * Returns true if the %Line begins with \c "BLOCK" or \c "DECAY"
   * followed by a block name. Comparison is done case-insensitive.
   */
  bool
  is_block_def() const
  {
    if (size() < 2) return false;

    const_iterator field = begin();
    return is_block_specifier(*field) && ((*++field)[0] != '#');
  }

  /** Returns true if the %Line begins with \c "#". */
  bool
  is_comment_line() const
  { return !empty() && (front()[0] == '#'); }

  /**
   * Returns true if the %Line is not empty and if it does not begin
   * with \c "#", \c "BLOCK" or \c "DECAY".
   */
  bool
  is_data_line() const
  { return !empty() && (front()[0] != '#') && !is_block_specifier(front()); }

  // capacity
  /** Returns the number of elements in the %Line. */
  size_type
  size() const
  { return impl_.size(); }

  /**
   * Returns the number of elements without the comment in the %Line.
   */
  size_type
  data_size() const
  {
    size_type data_size = 0;
    for (const_iterator field = begin(); field != end(); ++field)
    {
      if ((*field)[0] == '#') break;
      ++data_size;
    }
    return data_size;
  }

  /** Returns the size() of the largest possible %Line. */
  size_type
  max_size() const
  { return impl_.max_size(); }

  /** Returns true if the %Line is empty. */
  bool
  empty() const
  { return impl_.empty(); }

  // modifiers
  /**
   * \brief Swaps data with another %Line.
   * \param line %Line to be swapped with.
   */
  void
  swap(Line& line)
  {
    impl_.swap(line.impl_);
    bounds_.swap(line.bounds_);
    format_.swap(line.format_);
  }

  /** Erases all the elements in the %Line. */
  void
  clear()
  {
    impl_.clear();
    bounds_.clear();
    format_.clear();
  }

  /** Reformats the string representation of the %Line. */
  void
  reformat()
  {
    if (empty()) return;

    bounds_.clear();
    format_.clear();

    const_iterator field = begin();
    std::size_t pos1 = 0, pos2 = 0;

    if (is_block_specifier(*field))
    {
      pos1 = 0;
      pos2 = pos1 + field->length();
      bounds_.push_back(std::make_pair(pos1, pos2));

      if (++field == end()) return;

      pos1 = pos2 + 1;
      pos2 = pos1 + field->length();
      bounds_.push_back(std::make_pair(pos1, pos2));
    }
    else if ((*field)[0] == '#')
    {
      pos1 = 0;
      pos2 = pos1 + field->length();
      bounds_.push_back(std::make_pair(pos1, pos2));
    }
    else
    {
      pos1 = shift_width_;
      pos2 = pos1 + field->length();
      bounds_.push_back(std::make_pair(pos1, pos2));
    }

    while (++field != end())
    {
      pos1 = pos2 + calc_spaces_for_indent(pos2);
      if ((*field)[0] == '-' || (*field)[0] == '+') --pos1;
      pos2 = pos1 + field->length();
      bounds_.push_back(std::make_pair(pos1, pos2));
    }
  }

  /**
   * Comments the %Line. This function prefixes the %Line with a
   * \c "#" and packetizes all its elements into one.
   */
  void
  comment()
  { if (!empty()) str("#" + str()); }

  /**
   * Uncomments the %Line. This function removes the first character
   * of the %Line if it is a \c "#" and splits the former comment into
   * the corresponding number of fields.
   */
  void
  uncomment()
  {
    if (!empty() && front()[0] == '#')
    {
      front().erase(0, 1);
      str(str());
    }
  }

private:
  void
  build_format_str() const
  {
    if (empty()) return;

    std::stringstream format("");
    for (std::size_t i = 0; i < bounds_.size(); ++i)
    { format << " %|" << bounds_[i].first << "t|%" << (i+1) << "%"; }
    format_ = format.str().substr(1);
  }

  bool
  contains_comment() const
  {
    for (const_reverse_iterator field = rbegin(); field != rend(); ++field)
    { if ((*field)[0] == '#') return true; }
    return false;
  }

  static std::size_t
  calc_spaces_for_indent(const std::size_t& pos)
  {
    std::size_t width = shift_width_ - (pos % shift_width_);
    if (width < min_width_) width += shift_width_;
    return width;
  }

  static bool
  is_block_specifier(const value_type& arg)
  {
    // "BLOCK" and "DECAY" are both five characters long.
    if (arg.length() != 5) return false;

    const value_type arg_upper = boost::to_upper_copy(arg);
    return (arg_upper == "BLOCK") || (arg_upper == "DECAY");
  }

  template<class T> Line&
  insert_fundamental_type(const T& arg)
  {
    static const int digits = std::numeric_limits<T>::digits10;
    return *this << to_string(arg, digits);
  }

private:
  impl_type impl_;
  std::vector<std::pair<std::size_t, std::size_t> > bounds_;
  mutable std::string format_;

  static const std::size_t shift_width_ = 4;
  static const std::size_t min_width_   = 2;
};

template<> inline Line&
Line::operator<< <float>(const float& number)
{
  insert_fundamental_type(number);
  return *this;
}

template<> inline Line&
Line::operator<< <double>(const double& number)
{
  insert_fundamental_type(number);
  return *this;
}

template<> inline Line&
Line::operator<< <long double>(const long double& number)
{
  insert_fundamental_type(number);
  return *this;
}


/**
 * Container of \Lines that resembles a block in a SLHA structure.
 * This class is a named container of \Lines that resembles a block in
 * a SLHA structure. Unlike a block in a SLHA structure, a %Block can
 * contain any number of block definitions or it can be completely
 * empty.
 *
 * Access to the elements of the %Block is provided by the
 * operator[]() and at() functions. These take one or more strings
 * resp. ints as argument(s) and compare them against the first
 * strings of the contained \Lines (the ints are converted to strings
 * before comparison). The first Line that matches the provided
 * arguments is then returned, or if no matching Line is found, an
 * empty Line is appended to the %Block (operator[]()) or
 * \c std::out_of_range is thrown (at()). The special argument
 * \c "(any)" will be considered equal to all strings in the \Lines.
 * For example, <tt>at("(any)", "2")</tt> returns the first Line whose
 * second element is \c "2".
 */
class Block
{
private:
  typedef std::vector<Line> impl_type;

public:
  typedef std::vector<std::string>          key_type;
  typedef Line                              value_type;
  typedef Line&                             reference;
  typedef const Line&                       const_reference;
  typedef impl_type::iterator               iterator;
  typedef impl_type::const_iterator         const_iterator;
  typedef impl_type::reverse_iterator       reverse_iterator;
  typedef impl_type::const_reverse_iterator const_reverse_iterator;
  typedef impl_type::pointer                pointer;
  typedef impl_type::const_pointer          const_pointer;
  typedef impl_type::difference_type        difference_type;
  typedef impl_type::size_type              size_type;

  /**
   * \brief Constructs an empty %Block.
   * \param name Name of the %Block.
   */
  explicit
  Block(const std::string& name = "") : name_(name), impl_(0) {}

  // NOTE: The compiler-generated copy constructor and assignment
  //   operator for this class are just fine, so we don't need to
  //   write our own.

  /**
   * \brief Sets the name of the %Block.
   * \param newName New name of the %Block.
   *
   * Notice that this function only changes a property of the %Block.
   * No contained Line (in particular no block definition) is changed.
   */
  void
  name(const std::string& newName)
  { name_ = newName; }

  /** Returns the name of the %Block. */
  const std::string&
  name() const
  { return name_; }

  /**
   * \brief Assigns content from input stream to the %Block.
   * \param is Input stream to read content from.
   * \return Reference to \c *this.
   *
   * This functions reads non-empty lines from \p is, transforms them
   * into \Lines and adds them to the end of the %Block.
   */
  Block&
  read(std::istream& is)
  {
    std::string line_str;
    value_type line;
    bool nameless = name().empty();

    while (std::getline(is, line_str))
    {
      if (boost::all(line_str, boost::is_space())) continue;

      line.str(line_str);
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
   * \brief Assigns content to the %Block based on a string.
   * \param block String that is used as content for the %Block.
   * \return Reference to \c *this.
   *
   * This functions clears the content of the %Block and adds every
   * non-empty line found in \p block as Line to the end of the
   * %Block.
   */
  Block&
  str(const std::string& block)
  {
    std::stringstream input(block);
    clear();
    read(input);
    return *this;
  }

  /** Returns a string representation of the %Block. */
  std::string
  str() const
  {
    std::stringstream output("");
    output << *this;
    return output.str();
  }

  // element access
  /**
   * \brief Locates a Line in the %Block.
   * \param key First strings of the Line to be located.
   * \return Read/write reference to sought-after Line.
   *
   * This function takes a key (which is a vector of strings) and
   * locates the Line whose first strings are equal to the strings in
   * \p key. If no such Line exists, this function creates an empty
   * Line at the end of the %Block and returns a reference to it.
   */
  reference
  operator[](const key_type& key)
  {
    iterator line = find(key);
    if (line != end()) return *line;

    push_back(value_type());
    return back();
  }

  /**
   * \brief Locates a Line in the %Block.
   * \param key Integers that are used to locate the Line.
   * \return Read/write reference to sought-after Line.
   *
   * This function takes a key (which is a vector of ints) and locates
   * the Line whose first strings are equal to the to strings
   * converted ints in \p key. If no such Line exists, this function
   * creates an empty Line at the end of the %Block and returns a
   * reference to it.
   */
  reference
  operator[](const std::vector<int>& key)
  { return (*this)[cont_to_key(key)]; }

  /**
   * \brief Locates a Line in the %Block.
   * \param key String that is used to locate the Line.
   * \return Read/write reference to sought-after Line.
   *
   * This function locates the Line whose first string is equal to
   * \p key. If no such Line exists, this function creates an empty
   * Line at the end of the %Block and returns a reference to it.
   */
  reference
  operator[](const std::string& key)
  { return (*this)[key_type(1, key)]; }

  /**
   * \brief Locates a Line in the %Block.
   * \param key Integer that is used to locate the Line.
   * \return Read/write reference to sought-after Line.
   *
   * This function locates the Line whose first string is equal to the
   * to string converted \p key. If no such Line exists, this function
   * creates an empty Line at the end of the %Block and returns a
   * reference to it.
   */
  reference
  operator[](int key)
  { return (*this)[key_type(1, to_string(key))]; }

  /**
   * \brief Locates a Line in the %Block.
   * \param key First strings of the Line to be located.
   * \return Read/write reference to sought-after Line.
   * \throw std::out_of_range If \p key does not match any Line.
   *
   * This function takes a key (which is a vector of strings) and
   * locates the Line whose first strings are equal to the strings in
   * \p key. If no such Line exists, \c std::out_of_range is thrown.
   */
  reference
  at(const key_type& key)
  {
    iterator line = find(key);
    if (line != end()) return *line;

    throw std::out_of_range(
      "SLHAea::Block::at(‘" + boost::join(key, ",") + "’)");
  }

  /**
   * \brief Locates a Line in the %Block.
   * \param key First strings of the Line to be located.
   * \return Read-only (constant) reference to sought-after Line.
   * \throw std::out_of_range If \p key does not match any Line.
   *
   * This function takes a key (which is a vector of strings) and
   * locates the Line whose first strings are equal to the strings in
   * \p key. If no such Line exists, \c std::out_of_range is thrown.
   */
  const_reference
  at(const key_type& key) const
  {
    const_iterator line = find(key);
    if (line != end()) return *line;

    throw std::out_of_range(
      "SLHAea::Block::at(‘" + boost::join(key, ",") + "’)");
  }

  /**
   * \brief Locates a Line in the %Block.
   * \param key Integers that are used to locate the Line.
   * \return Read/write reference to sought-after Line.
   * \throw std::out_of_range If \p key does not match any Line.
   *
   * This function takes a key (which is a vector of ints) and locates
   * the Line whose first strings are equal to the to strings
   * converted ints in \p key. If no such Line exists,
   * \c std::out_of_range is thrown.
   */
  reference
  at(const std::vector<int>& key)
  { return at(cont_to_key(key)); }

  /**
   * \brief Locates a Line in the %Block.
   * \param key Integers that are used to locate the Line.
   * \return Read-only (constant) reference to sought-after Line.
   * \throw std::out_of_range If \p key does not match any Line.
   *
   * This function takes a key (which is a vector of ints) and locates
   * the Line whose first strings are equal to the to strings
   * converted ints in \p key. If no such Line exists,
   * \c std::out_of_range is thrown.
   */
  const_reference
  at(const std::vector<int>& key) const
  { return at(cont_to_key(key)); }

  /**
   * \brief Locates a Line in the %Block.
   * \param s0, s1, s2, s3, s4 First strings of the Line to be
   *   located.
   * \return Read/write reference to sought-after Line.
   * \throw std::out_of_range If provided strings do not match any
   *   Line.
   *
   * This function takes up to five strings and locates the Line whose
   * first strings are equal to all provided non-empty strings. If no
   * such Line exists, \c std::out_of_range is thrown.
   */
  reference
  at(const std::string& s0,      const std::string& s1 = "",
     const std::string& s2 = "", const std::string& s3 = "",
     const std::string& s4 = "")
  { return at(strings_to_key(s0, s1, s2, s3, s4)); }

  /**
   * \brief Locates a Line in the %Block.
   * \param s0, s1, s2, s3, s4 First strings of the Line to be
   *   located.
   * \return Read-only (constant) reference to sought-after Line.
   * \throw std::out_of_range If provided strings do not match any
   *   Line.
   *
   * This function takes up to five strings and locates the Line whose
   * first strings are equal to all provided non-empty strings. If no
   * such Line exists, \c std::out_of_range is thrown.
   */
  const_reference
  at(const std::string& s0,      const std::string& s1 = "",
     const std::string& s2 = "", const std::string& s3 = "",
     const std::string& s4 = "") const
  { return at(strings_to_key(s0, s1, s2, s3, s4)); }

  /**
   * \brief Locates a Line in the %Block.
   * \param i0, i1, i2, i3, i4 Integers that are used to locate the
   *   Line.
   * \return Read/write reference to sought-after Line.
   * \throw std::out_of_range If provided ints do not match any
   *   Line.
   *
   * This function takes up to five ints and locates the Line whose
   * first strings are equal to all provided to string converted ints.
   * If no such Line exists, \c std::out_of_range is thrown.
   */
  reference
  at(int i0, int i1 = no_ind, int i2 = no_ind, int i3 = no_ind,
     int i4 = no_ind)
  { return at(ints_to_key(i0, i1, i2, i3, i4)); }

  /**
   * \brief Locates a Line in the %Block.
   * \param i0, i1, i2, i3, i4 Integers that are used to locate the
   *   Line.
   * \return Read-only (constant) reference to sought-after Line.
   * \throw std::out_of_range If provided ints do not match any Line.
   *
   * This function takes up to five ints and locates the Line whose
   * first strings are equal to all provided to string converted ints.
   * If no such Line exists, \c std::out_of_range is thrown.
   */
  const_reference
  at(int i0, int i1 = no_ind, int i2 = no_ind, int i3 = no_ind,
     int i4 = no_ind) const
  { return at(ints_to_key(i0, i1, i2, i3, i4)); }

  /**
   * Returns a read/write reference to the first element of the
   * %Block.
   */
  reference
  front()
  { return impl_.front(); }

  /**
   * Returns a read-only (constant) reference to the first element of
   * the %Block.
   */
  const_reference
  front() const
  { return impl_.front(); }

  /**
   * Returns a read/write reference to the last element of the %Block.
   */
  reference
  back()
  { return impl_.back(); }

  /**
   * Returns a read-only (constant) reference to the last element of
   * the %Block.
   */
  const_reference
  back() const
  { return impl_.back(); }

  // iterators
  /**
   * Returns a read/write iterator that points to the first element in
   * the %Block. Iteration is done in ordinary element order.
   */
  iterator
  begin()
  { return impl_.begin(); }

  /**
   * Returns a read-only (constant) iterator that points to the first
   * element in the %Block. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  begin() const
  { return impl_.begin(); }

  /**
   * Returns a read-only (constant) iterator that points to the first
   * element in the %Block. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  cbegin() const
  { return impl_.begin(); }

  /**
   * Returns a read/write iterator that points one past the last
   * element in the %Block. Iteration is done in ordinary element
   * order.
   */
  iterator
  end()
  { return impl_.end(); }

  /**
   * Returns a read-only (constant) iterator that points one past the
   * last element in the %Block. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  end() const
  { return impl_.end(); }

  /**
   * Returns a read-only (constant) iterator that points one past the
   * last element in the %Block. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  cend() const
  { return impl_.end(); }

  /**
   * Returns a read/write reverse iterator that points to the last
   * element in the %Block. Iteration is done in reverse element
   * order.
   */
  reverse_iterator
  rbegin()
  { return impl_.rbegin(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * the last element in the %Block. Iteration is done in reverse
   * element order.
   */
  const_reverse_iterator
  rbegin() const
  { return impl_.rbegin(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * the last element in the %Block. Iteration is done in reverse
   * element order.
   */
  const_reverse_iterator
  crbegin() const
  { return impl_.rbegin(); }

  /**
   * Returns a read/write reverse iterator that points to one before
   * the first element in the %Block. Iteration is done in reverse
   * element order.
   */
  reverse_iterator
  rend()
  { return impl_.rend(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * one before the first element in the %Block. Iteration is done
   * in reverse element order.
   */
  const_reverse_iterator
  rend() const
  { return impl_.rend(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * one before the first element in the %Block. Iteration is done
   * in reverse element order.
   */
  const_reverse_iterator
  crend() const
  { return impl_.rend(); }

  // operations
  /**
   * \brief Tries to locate a Line in the %Block.
   * \param key First strings of the Line to be located.
   * \return Read/write iterator pointing to sought-after element, or
   *   end() if not found.
   *
   * This function takes a key (which is a vector of strings) and
   * tries to locate the Line whose first strings are equal to the
   * strings in \p key. If successful the function returns a
   * read/write iterator pointing to the sought after Line. If
   * unsuccessful it returns end().
   */
  iterator
  find(const key_type& key)
  { return std::find_if(begin(), end(), line_matches(key)); }

  /**
   * \brief Tries to locate a Line in the %Block.
   * \param key First strings of the Line to be located.
   * \return Read-only (constant) iterator pointing to sought-after
   *   element, or end() const if not found.
   *
   * This function takes a key (which is a vector of strings) and
   * tries to locate the Line whose first strings are equal to the
   * strings in \p key. If successful the function returns a read-only
   * (constant) iterator pointing to the sought after Line. If
   * unsuccessful it returns end() const.
   */
  const_iterator
  find(const key_type& key) const
  { return std::find_if(begin(), end(), line_matches(key)); }

  // introspection
  /**
   * \brief Counts all \Lines that match a given key.
   * \param key First strings of the \Lines that will be counted.
   * \return Number of lines whose first strings equal \p key.
   */
  size_type
  count(const key_type& key) const
  { return std::count_if(begin(), end(), line_matches(key)); }

  // capacity
  /** Returns the number of elements in the %Block. */
  size_type
  size() const
  { return impl_.size(); }

  /** Returns the size() of the largest possible %Block. */
  size_type
  max_size() const
  { return impl_.max_size(); }

  /** Returns true if the %Block is empty. */
  bool
  empty() const
  { return impl_.empty(); }

  // modifiers
  /**
   * \brief Adds a Line to the end of the %Block.
   * \param line Line to be added.
   *
   * This function creates an element at the end of the %Block and
   * assigns the given \p line to it.
   */
  void
  push_back(const value_type& line)
  { impl_.push_back(line); }

  /**
   * \brief Adds a Line to the end of the %Block.
   * \param line String that is used to construct the Line that will
   *   be added.
   *
   * This function creates an element at the end of the %Block and
   * assigns the Line that is constructed from \p line to it.
   */
  void
  push_back(const std::string& line)
  { impl_.push_back(value_type(line)); }

  /**
   * Removes the last element. This function shrinks the size() of the
   * %Block by one.
   */
  void
  pop_back()
  { impl_.pop_back(); }

  /**
   * \brief Inserts a Line before given \p position.
   * \param position Iterator into the %Block.
   * \param line Line to be inserted.
   * \return Iterator pointing to the inserted element.
   *
   * This function inserts a copy of \p line before the specified
   * \p position and thus enlarges the %Block by one.
   */
  iterator
  insert(iterator position, const value_type& line)
  { return impl_.insert(position, line); }

  /**
   * \brief Inserts a range into the %Block.
   * \param position Iterator into the %Block.
   * \param first Input iterator.
   * \param last Input iterator.
   *
   * This function inserts copies of the \Lines in the range
   * [\p first, \p last) into the %Block before the specified
   * \p position and thus enlarges the %Block accordingly.
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
   * thus shortens the %Block by one.
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
   * \p last) and shortens the %Block accordingly.
   */
  iterator
  erase(iterator first, iterator last)
  { return impl_.erase(first, last); }

  /**
   * \brief Swaps data with another %Block.
   * \param block %Block to be swapped with.
   */
  void
  swap(Block& block)
  {
    name_.swap(block.name_);
    impl_.swap(block.impl_);
  }

  /**
   * Erases all the elements in the %Block and set its name to an
   * empty string.
   */
  void
  clear()
  {
    name_.clear();
    impl_.clear();
  }

  /**
   * \brief Reformats all \Lines in the %Block.
   * \sa Line::reformat()
   */
  void
  reformat()
  { std::for_each(begin(), end(), std::mem_fun_ref(&value_type::reformat)); }

  /**
   * \brief Comments all \Lines in the %Block.
   * \sa Line::comment()
   */
  void
  comment()
  { std::for_each(begin(), end(), std::mem_fun_ref(&value_type::comment)); }

  /**
   * \brief Uncomments all \Lines in the %Block.
   * \sa Line::uncomment()
   */
  void
  uncomment()
  { std::for_each(begin(), end(), std::mem_fun_ref(&value_type::uncomment)); }

private:
  template<class Container> static key_type
  cont_to_key(const Container& cont)
  {
    key_type key;
    key.reserve(cont.size());
    std::transform(cont.begin(), cont.end(), std::back_inserter(key),
      boost::lexical_cast<std::string, typename Container::value_type>);
    return key;
  }

  static key_type
  strings_to_key(const std::string& s0, const std::string& s1,
                 const std::string& s2, const std::string& s3,
                 const std::string& s4)
  {
    key_type key;
    key.reserve(5);
    if (s0.empty()) return key; key.push_back(s0);
    if (s1.empty()) return key; key.push_back(s1);
    if (s2.empty()) return key; key.push_back(s2);
    if (s3.empty()) return key; key.push_back(s3);
    if (s4.empty()) return key; key.push_back(s4);
    return key;
  }

  static key_type
  ints_to_key(int i0, int i1, int i2, int i3, int i4)
  {
    key_type key;
    key.reserve(5);
    if (i0 == no_ind) return key; key.push_back(to_string(i0));
    if (i1 == no_ind) return key; key.push_back(to_string(i1));
    if (i2 == no_ind) return key; key.push_back(to_string(i2));
    if (i3 == no_ind) return key; key.push_back(to_string(i3));
    if (i4 == no_ind) return key; key.push_back(to_string(i4));
    return key;
  }

  struct line_matches : public std::unary_function<value_type, bool>
  {
    explicit
    line_matches(const key_type& key) : key_(key) {}

    bool
    operator()(const value_type& line) const
    {
      if (key_.empty() || key_.size() > line.size()) return false;

      return std::equal(key_.begin(), key_.end(), line.begin(),
                        index_iequals);
    }

  private:
    static bool
    index_iequals(const std::string& a, const std::string& b)
    { return (a == "(any)") || boost::iequals(a, b); }

  private:
    key_type key_;
  };

private:
  std::string name_;
  impl_type impl_;
  static const int no_ind = -32768;
};


/**
 * Container of \Blocks that resembles a complete SLHA structure.
 * This class is a container of \Blocks that resembles a complete
 * SLHA structure. Its name is an abbreviation of "collection". The
 * elements of Coll objects can be accessed via their names with the
 * operator[]() and at() functions and access to single fields is
 * provided by the field() functions. To fill this container, the
 * functions read() or str() can be used which read data from an input
 * stream or a string, respectively.
 */
class Coll
{
private:
  typedef std::deque<Block> impl_type;

public:
  typedef std::string                       key_type;
  typedef Block                             value_type;
  typedef Block&                            reference;
  typedef const Block&                      const_reference;
  typedef impl_type::iterator               iterator;
  typedef impl_type::const_iterator         const_iterator;
  typedef impl_type::reverse_iterator       reverse_iterator;
  typedef impl_type::const_reverse_iterator const_reverse_iterator;
  typedef impl_type::pointer                pointer;
  typedef impl_type::const_pointer          const_pointer;
  typedef impl_type::difference_type        difference_type;
  typedef impl_type::size_type              size_type;

  /** Constructs an empty %Coll. */
  Coll() : impl_(0) {}

  // NOTE: The compiler-generated copy constructor and assignment
  //   operator for this class are just fine, so we don't need to
  //   write our own.

  /**
   * \brief Constructs a %Coll with content from input stream.
   * \param is Input stream to read content from.
   * \sa read()
   */
  explicit
  Coll(std::istream& is)
  { read(is); }

  /**
   * \brief Accesses a single field in the %Coll.
   * \param key Key that refers to the field that should be accessed.
   * \return Read/write reference to the field referred to by \p key.
   * \throw std::out_of_range If \p key refers to a non-existing
   *   field.
   */
  Line::reference
  field(const Key& key);

  /**
   * \brief Accesses a single field in the %Coll.
   * \param key Key that refers to the field that should be accessed.
   * \return Read-only (constant) reference to the field referred to
   *   by \p key.
   * \throw std::out_of_range If \p key refers to a non-existing
   *   field.
   */
  Line::const_reference
  field(const Key& key) const;

  /**
   * \brief Assigns content from input stream to the %Coll.
   * \param is Input stream to read content from.
   * \returns Reference to \c *this.
   *
   * This function reads non-empty lines from \p is, transforms them
   * into \Lines, which are collected into the corresponding \Blocks
   * that are added to the %Coll.
   */
  Coll&
  read(std::istream& is)
  {
    std::string line_str;
    Line line;

    const size_type orig_size = size();
    pointer block = push_back_named_block("");

    while (std::getline(is, line_str))
    {
      if (boost::all(line_str, boost::is_space())) continue;

      line.str(line_str);
      if (line.is_block_def()) block = push_back_named_block(line[1]);
      block->push_back(line);
    }

    erase_if_empty("", orig_size);
    return *this;
  }

  /**
   * \brief Assigns content to the %Coll based on a string.
   * \param coll String that is used as content for the %Coll.
   * \returns Reference to \c *this.
   */
  Coll&
  str(const std::string& coll)
  {
    std::stringstream input(coll);
    clear();
    read(input);
    return *this;
  }

  /** Returns a string representation of the %Coll. */
  std::string
  str() const
  {
    std::stringstream output("");
    output << *this;
    return output.str();
  }

  // element access
  /**
   * \brief Locates a Block in the %Coll.
   * \param blockName Name of the Block to be located.
   * \return Read/write reference to sought-after Block.
   *
   * This function locates the first Block whose name equals
   * \p blockName. If no such Block is present, an empty Block with
   * this name is added to the end of the %Coll and a reference to it
   * is then returned.
   */
  reference
  operator[](const key_type& blockName)
  {
    iterator block = find(blockName);
    if (block != end()) return *block;

    push_back(value_type(blockName));
    return back();
  }

  /**
   * \brief Locates a Block in the %Coll.
   * \param blockName Name of the Block to be located.
   * \return Read/write reference to sought-after Block.
   * \throw std::out_of_range If no Block with the name \p blockName
   *   exists.
   */
  reference
  at(const key_type& blockName)
  {
    iterator block = find(blockName);
    if (block != end()) return *block;

    throw std::out_of_range("SLHAea::Coll::at(‘" + blockName + "’)");
  }

  /**
   * \brief Locates a Block in the %Coll.
   * \param blockName Name of the Block to be located.
   * \return Read-only (constant) reference to sought-after Block.
   * \throw std::out_of_range If no Block with the name \p blockName
   *   exists.
   */
  const_reference
  at(const key_type& blockName) const
  {
    const_iterator block = find(blockName);
    if (block != end()) return *block;

    throw std::out_of_range("SLHAea::Coll::at(‘" + blockName + "’)");
  }

  /**
   * Returns a read/write reference to the first element of the %Coll.
   */
  reference
  front()
  { return impl_.front(); }

  /**
   * Returns a read-only (constant) reference to the first element of
   * the %Coll.
   */
  const_reference
  front() const
  { return impl_.front(); }

  /**
   * Returns a read/write reference to the last element of the %Coll.
   */
  reference
  back()
  { return impl_.back(); }

  /**
   * Returns a read-only (constant) reference to the last element of
   * the %Coll.
   */
  const_reference
  back() const
  { return impl_.back(); }

  // iterators
  /**
   * Returns a read/write iterator that points to the first element in
   * the %Coll. Iteration is done in ordinary element order.
   */
  iterator
  begin()
  { return impl_.begin(); }

  /**
   * Returns a read-only (constant) iterator that points to the first
   * element in the %Coll. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  begin() const
  { return impl_.begin(); }

  /**
   * Returns a read-only (constant) iterator that points to the first
   * element in the %Coll. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  cbegin() const
  { return impl_.begin(); }

  /**
   * Returns a read/write iterator that points one past the last
   * element in the %Coll. Iteration is done in ordinary element
   * order.
   */
  iterator
  end()
  { return impl_.end(); }

  /**
   * Returns a read-only (constant) iterator that points one past the
   * last element in the %Coll. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  end() const
  { return impl_.end(); }

  /**
   * Returns a read-only (constant) iterator that points one past the
   * last element in the %Coll. Iteration is done in ordinary element
   * order.
   */
  const_iterator
  cend() const
  { return impl_.end(); }

  /**
   * Returns a read/write reverse iterator that points to the last
   * element in the %Coll. Iteration is done in reverse element order.
   */
  reverse_iterator
  rbegin()
  { return impl_.rbegin(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * the last element in the %Coll. Iteration is done in reverse
   * element order.
   */
  const_reverse_iterator
  rbegin() const
  { return impl_.rbegin(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * the last element in the %Coll. Iteration is done in reverse
   * element order.
   */
  const_reverse_iterator
  crbegin() const
  { return impl_.rbegin(); }

  /**
   * Returns a read/write reverse iterator that points to one before
   * the first element in the %Coll. Iteration is done in reverse
   * element order.
   */
  reverse_iterator
  rend()
  { return impl_.rend(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * one before the first element in the %Coll. Iteration is done in
   * reverse element order.
   */
  const_reverse_iterator
  rend() const
  { return impl_.rend(); }

  /**
   * Returns a read-only (constant) reverse iterator that points to
   * one before the first element in the %Coll. Iteration is done in
   * reverse element order.
   */
  const_reverse_iterator
  crend() const
  { return impl_.rend(); }

  // operations
  /**
   * \brief Tries to locate a Block in the %Coll.
   * \param blockName Name of the Block to be located.
   * \return Read/write iterator pointing to sought-after element, or
   *   end() if not found.
   *
   * This function takes a key and tries to locate the Block whose
   * name matches \p blockName (comparison is case-insensitive). If
   * successful the function returns a read/write iterator pointing to
   * the sought after Block. If unsuccessful it returns end().
   */
  iterator
  find(const key_type& blockName)
  { return std::find_if(begin(), end(), name_iequals(blockName)); }

  /**
   * \brief Tries to locate a Block in the %Coll.
   * \param blockName Name of the Block to be located.
   * \return Read-only (constant) iterator pointing to sought-after
   *   element, or end() const if not found.
   *
   * This function takes a key and tries to locate the Block whose
   * name matches \p blockName (comparison is case-insensitive). If
   * successful the function returns a read-only (constant) iterator
   * pointing to the sought after Block. If unsuccessful it returns
   * end() const.
   */
  const_iterator
  find(const key_type& blockName) const
  { return std::find_if(begin(), end(), name_iequals(blockName)); }

  // introspection
  /**
   * \brief Counts all \Blocks with a given name.
   * \param blockName Name of the \Blocks that will be counted.
   * \return Number of blocks whose name equals \p blockName.
   *
   * Notice that the comparison of \p blockName and the names of the
   * \Blocks is case-insensitive.
   */
  size_type
  count(const key_type& blockName) const
  { return std::count_if(begin(), end(), name_iequals(blockName)); }

  // capacity
  /** Returns the number of elements in the %Coll. */
  size_type
  size() const
  { return impl_.size(); }

  /** Returns the size() of the largest possible %Coll. */
  size_type
  max_size() const
  { return impl_.max_size(); }

  /** Returns true if the %Coll is empty. */
  bool
  empty() const
  { return impl_.empty(); }

  // modifiers
  /**
   * \brief Adds a Block to the end of the %Coll.
   * \param block Block to be added.
   *
   * This function creates an element at the end of the %Coll and
   * assigns the given \p block to it.
   */
  void
  push_back(const value_type& block)
  { impl_.push_back(block); }

  /**
   * \brief Adds a Block to the end of the %Coll.
   * \param blockString String that is used to construct the Block
   *   that will be added.
   *
   * This function creates an element at the end of the %Coll and
   * assigns the Block that is constructed from \p blockString to it.
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
   * %Coll by one.
   */
  void
  pop_back()
  { impl_.pop_back(); }

  /**
   * \brief Inserts a Block before given \p position.
   * \param position Iterator into the %Coll.
   * \param block Block to be inserted.
   * \return Iterator pointing to the inserted element.
   *
   * This function inserts a copy of \p block before the specified
   * \p position and thus enlarges the %Coll by one.
   */
  iterator
  insert(iterator position, const value_type& block)
  { return impl_.insert(position, block); }

  /**
   * \brief Inserts a range into the %Coll.
   * \param position Iterator into the %Coll.
   * \param first Input iterator.
   * \param last Input iterator.
   *
   * This function inserts copies of the \Blocks in the range
   * [\p first, \p last) into the %Coll before the specified
   * \p position and thus enlarges the %Coll accordingly.
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
   * thus shortens the %Coll by one.
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
   * \p last) and shortens the %Coll accordingly.
   */
  iterator
  erase(iterator first, iterator last)
  { return impl_.erase(first, last); }

  /**
   * \brief Tries to erase a Block in the %Coll.
   * \param blockName Name of the Block to be erased.
   * \return Iterator pointing to the next element (or end()).
   *
   * This function takes a key and tries to erase the Block whose
   * name matches \p blockName (comparison is case-insensitive). If
   * the %Coll contains such Block, the function returns an iterator
   * pointing to the next element (or end()). If no such Block exists,
   * end() is returned.
   */
  iterator
  erase(const key_type& blockName)
  {
    iterator block = find(blockName);
    if (block != end()) return erase(block);
    return block;
  }

  /**
   * \brief Swaps data with another %Coll.
   * \param coll %Coll to be swapped with.
   */
  void
  swap(Coll& coll)
  { impl_.swap(coll.impl_); }

  /** Erases all the elements in the %Coll. */
  void
  clear()
  { impl_.clear(); }

  /**
   * \brief Reformats all \Blocks in the %Coll.
   * \sa Block::reformat()
   */
  void
  reformat()
  { std::for_each(begin(), end(), std::mem_fun_ref(&value_type::reformat)); }

  /**
   * \brief Comments all \Blocks in the %Coll.
   * \sa Block::comment()
   */
  void
  comment()
  { std::for_each(begin(), end(), std::mem_fun_ref(&value_type::comment)); }

  /**
   * \brief Uncomments all \Blocks in the %Coll.
   * \sa Block::uncomment()
   */
  void
  uncomment()
  { std::for_each(begin(), end(), std::mem_fun_ref(&value_type::uncomment)); }

private:
  struct name_iequals : public std::unary_function<value_type, bool>
  {
    explicit
    name_iequals(const key_type& blockName) : name_(blockName) {}

    bool
    operator()(const value_type& block) const
    { return boost::iequals(block.name(), name_); }

  private:
    key_type name_;
  };

  pointer
  push_back_named_block(const key_type& blockName)
  {
    push_back(value_type(blockName));
    return &back();
  }

  iterator
  erase_if_empty(const key_type& blockName, const size_type& offset = 0)
  {
    iterator block =
        std::find_if(begin() + offset, end(), name_iequals(blockName));
    if (block != end() && block->empty()) return erase(block);
    return block;
  }

private:
  impl_type impl_;
};


/**
 * Reference to a single field in a SLHA structure.
 * This data type represents a reference to a single field in a SLHA
 * structure, but which is independent of any concrete Coll object.
 * That means that only the keys and the index of the Coll, Block, and
 * Line containers of the corresponding field are stored. One of the
 * main purposes of this data type is the conversion to string and
 * vice versa in a way that the string representation of a %Key can be
 * used as a single field in a SLHA structure. For example, the string
 * representation of a %Key that refers to the entry in the first row
 * and third column of the RVHMIX matrix is \c "RVHMIX;1,3;2". Further
 * examples are \c "1000022;DECAY;2" which refers to the total decay
 * width of the lightest neutralino or \c "1000022;(any),2,11,24;0"
 * which refers to the branching ratio of the decay of the lightest
 * neutralino into an electron and a W boson.
 */
struct Key
{
public:
  /** Name of the Block that contains the field. */
  Coll::key_type block;

  /** First field(s) of the Line that contains the field. */
  Block::key_type line;

  /** Index of the field in the Line. */
  Line::size_type field;

  /**
   * \brief Constructs a %Key from explicit key values.
   * \param _block Name of the Block that contains the field.
   * \param _line First field(s) of the Line that contains the field.
   * \param _field Index of the field in the Line.
   */
  Key(const Coll::key_type& _block,
      const Block::key_type& _line,
      const Line::size_type& _field)
    : block(_block), line(_line), field(_field) {}

  /**
   * \brief Constructs a %Key from a string.
   * \param keyString String from which the %Key is constructed.
   * \sa str()
   */
  Key(const std::string& keyString)
  { str(keyString); }

  /**
   * \brief Constructs a %Key from a string.
   * \param keyString String from which the %Key is constructed.
   * \sa str()
   */
  Key(const char* keyString)
  { str(keyString); }

  /**
   * \brief Converts a string to a %Key.
   * \param keyString String that represents a %Key.
   * \return Reference to \c *this.
   */
  Key&
  str(const std::string& keyString)
  {
    std::vector<std::string> keys;
    boost::split(keys, keyString, boost::is_any_of(";"));

    if (keys.size() != 3)
    { throw std::invalid_argument("SLHAea::Key::str(‘" + keyString + "’)"); }

    clear();
    block = keys[0];
    boost::split(line, keys[1], boost::is_any_of(","));
    field = boost::lexical_cast<Line::size_type>(keys[2]);

    return *this;
  }

  /**
   * \brief Converts a %Key into its string representation.
   * \return String that represents the %Key.
   */
  std::string
  str() const
  {
    std::stringstream output("");
    output << block << ";" << boost::join(line, ",") << ";" << field;
    return output.str();
  }

private:
  void
  clear()
  {
    block.clear();
    line.clear();
    field = 0;
  }
};


inline Line::reference
Coll::field(const Key& key)
{ return at(key.block).at(key.line).at(key.field); }

inline Line::const_reference
Coll::field(const Key& key) const
{ return at(key.block).at(key.line).at(key.field); }


// stream operators
inline std::istream&
operator>>(std::istream& is, Block& block)
{
  block.read(is);
  return is;
}

inline std::istream&
operator>>(std::istream& is, Coll& coll)
{
  coll.read(is);
  return is;
}

inline std::ostream&
operator<<(std::ostream& os, const Line& line)
{ return os << line.str(); }

inline std::ostream&
operator<<(std::ostream& os, const Block& block)
{
  std::copy(block.begin(), block.end(),
            std::ostream_iterator<Block::value_type>(os, "\n"));
  return os;
}

inline std::ostream&
operator<<(std::ostream& os, const Coll& coll)
{
  std::copy(coll.begin(), coll.end(),
            std::ostream_iterator<Coll::value_type>(os));
  return os;
}

inline std::ostream&
operator<<(std::ostream& os, const Key& key)
{ return os << key.str(); }


// relational operators for Line
inline bool
operator==(const Line& a, const Line& b)
{
  return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin())
    && a.str() == b.str();
}

inline bool
operator<(const Line& a, const Line& b)
{
  bool a_is_block_def = a.is_block_def();

  if (a_is_block_def == b.is_block_def())
  {
    return std::lexicographical_compare(a.begin(), a.end(),
                                        b.begin(), b.end());
  }
  return a_is_block_def;
}

inline bool
operator!=(const Line& a, const Line& b)
{ return !(a == b); }

inline bool
operator>(const Line& a, const Line& b)
{ return b < a; }

inline bool
operator<=(const Line& a, const Line& b)
{ return !(b < a); }

inline bool
operator>=(const Line& a, const Line& b)
{ return !(a < b); }


// relational operators for Block
inline bool
operator==(const Block& a, const Block& b)
{
  return a.size() == b.size() && a.name() == b.name()
    && std::equal(a.begin(), a.end(), b.begin());
}

inline bool
operator<(const Block& a, const Block& b)
{
  if (a.name() != b.name()) return a.name() < b.name();

  return std::lexicographical_compare(a.begin(), a.end(),
                                      b.begin(), b.end());
}

inline bool
operator!=(const Block& a, const Block& b)
{ return !(a == b); }

inline bool
operator>(const Block& a, const Block& b)
{ return b < a; }

inline bool
operator<=(const Block& a, const Block& b)
{ return !(b < a); }

inline bool
operator>=(const Block& a, const Block& b)
{ return !(a < b); }


// relational operators for Coll
inline bool
operator==(const Coll& a, const Coll& b)
{ return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin()); }

inline bool
operator<(const Coll& a, const Coll& b)
{
  return std::lexicographical_compare(a.begin(), a.end(),
                                      b.begin(), b.end());
}

inline bool
operator!=(const Coll& a, const Coll& b)
{ return !(a == b); }

inline bool
operator>(const Coll& a, const Coll& b)
{ return b < a; }

inline bool
operator<=(const Coll& a, const Coll& b)
{ return !(b < a); }

inline bool
operator>=(const Coll& a, const Coll& b)
{ return !(a < b); }

} // namespace SLHAea

#endif // SLHAEA_H

// vim: sw=2 tw=78
