// SLHApp - SUSY Les Houches Accord plus plus
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

#ifndef SLHAPP_TESTSLHABLOCK_H
#define SLHAPP_TESTSLHABLOCK_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <slhapp.h>

using namespace std;

namespace SLHApp {

class TestSLHABlock : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestSLHABlock);
  CPPUNIT_TEST(testConstructors);
  //CPPUNIT_TEST(testAccessors);
  //CPPUNIT_TEST(testModifiers);
  //CPPUNIT_TEST(testIterators);
  CPPUNIT_TEST(testMiscellaneous);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}

  void tearDown() {}

  void testConstructors()
  {
    SLHABlock b1, b2("test");
    CPPUNIT_ASSERT(b1.name() == "");
    CPPUNIT_ASSERT(b2.name() == "test");

    b1.name("test");
    b2.name("");
    CPPUNIT_ASSERT(b1.name() == "test");
    CPPUNIT_ASSERT(b2.name() == "");
  }

  void testMiscellaneous()
  {
    SLHABlock b1;
    CPPUNIT_ASSERT(b1.name() == "");
    b1.name("test");
    CPPUNIT_ASSERT(b1.name() == "test");
    CPPUNIT_ASSERT(b1.size() == 0);

    b1.push_back(SLHALine(" 1 a b c"));
    CPPUNIT_ASSERT(b1.front().str() == " 1 a b c");
    CPPUNIT_ASSERT(b1.back().str() == " 1 a b c");

    b1.push_back(SLHALine(" 2 a b"));
    CPPUNIT_ASSERT(b1.front().str() == " 1 a b c");
    CPPUNIT_ASSERT(b1.back().str() == " 2 a b");
    CPPUNIT_ASSERT(b1.str() == " 1 a b c\n 2 a b\n");

    b1[""] = " 2 a c";
    vector<int> vi;
    vector<string> vs;
    vi.push_back(1);
    vs.push_back("1");
    CPPUNIT_ASSERT(b1[vi].str() == " 1 a b c");
    CPPUNIT_ASSERT(b1[vs].str() == " 1 a b c");
    CPPUNIT_ASSERT(b1["2 a"].str() == " 2 a b");
    CPPUNIT_ASSERT(b1["2 a c"].str() == " 2 a c");
    CPPUNIT_ASSERT(b1["2 a c"][2] == "c");
    CPPUNIT_ASSERT(b1.at("2", "a", "c")[2] == "c");
    b1.at() = " 3 -1 y zzz";
    CPPUNIT_ASSERT(b1.at("3").str() == " 3 -1 y zzz");
    CPPUNIT_ASSERT(b1.at(3,-1).str() == " 3 -1 y zzz");
    CPPUNIT_ASSERT(b1.push_back(" 4 1 f g").back().str() == " 4 1 f g");

    std::string block = " 1 a b c \n"
                        " 2 d e f # comment\n"
                        " 3 1 g h i   \n"
                        " 3 2 gg hh ii";
    std::string block_bh =
                        " 1 a b c\n"
                        " 2 d e f # comment\n"
                        " 3 1 g h i\n"
                        " 3 2 gg hh ii\n";

    b1.str(block);
    CPPUNIT_ASSERT(b1.size() == 4);
    CPPUNIT_ASSERT(b1["3 2"][2] == "gg");
    CPPUNIT_ASSERT(b1.str() == block_bh);
  }
};

} // namespace SLHApp

#endif // SLHAPP_TESTSLHABLOCK_H

// vim: sw=2 tw=78
