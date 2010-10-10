#!/usr/bin/env ruby

require 'test/unit'
require 'slhaea'

class TC_Line < Test::Unit::TestCase
  def test_constructors

    l1 = Line.new ""
    l1[1] = "1"
    return

    assert_equal true, l1.empty?
    assert_equal 0, l1.size
    assert_equal "", l1.to_s

    l1 = Line.new " \t \v \f \r \n"
    assert_equal true, l1.empty?
    assert_equal 0, l1.size
    assert_equal "", l1.to_s

    l1 = Line.new "1 2 3"
    assert_equal false, l1.empty?
    assert_equal 3, l1.size
    assert_equal "1 2 3", l1.to_s

    l1 = Line.new " 1  2  3 \n 4  5 "
    assert_equal false, l1.empty?
    assert_equal 3, l1.size
    assert_equal " 1  2  3", l1.to_s
  end
end
