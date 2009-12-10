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

#ifndef SLHAEA_TESTSLHABLOCK_H
#define SLHAEA_TESTSLHABLOCK_H

#include <string>
#include <vector>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <slhaea.h>

using namespace std;

namespace SLHAea {

class TestSLHABlock : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestSLHABlock);
  CPPUNIT_TEST(testConstructors);
  CPPUNIT_TEST(testAccessors);
  CPPUNIT_TEST(testModifiers);
  CPPUNIT_TEST(testIterators);
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

  void testAccessors()
  {
    SLHABlock b1("test");
    b1[""] = " 1 1 # 11";
    b1[""] = " 1 2 # 12";
    b1[""] = " 2 1 # 21";
    const SLHABlock cb1 = b1;

    string out = " 1 1 # 11\n"
                 " 1 2 # 12\n"
                 " 2 1 # 21\n";
    CPPUNIT_ASSERT(b1.str() == out);

    CPPUNIT_ASSERT(b1["1 2"][2] == "# 12");
    CPPUNIT_ASSERT(b1.front().str() == " 1 1 # 11");
    CPPUNIT_ASSERT(b1.back().str() == " 2 1 # 21");
    CPPUNIT_ASSERT(b1.back().str() == b1.at(2,1).str());

    CPPUNIT_ASSERT(cb1["1 2"][2] == "# 12");
    CPPUNIT_ASSERT(cb1.front().str() == " 1 1 # 11");
    CPPUNIT_ASSERT(cb1.back().str() == " 2 1 # 21");
    CPPUNIT_ASSERT(cb1.back().str() == cb1.at(2,1).str());

    vector<string> vs;
    vector<int> vi;
    vs.push_back("1");
    vs.push_back("2");
    vi.push_back(1);
    vi.push_back(2);
    CPPUNIT_ASSERT(b1["1 2"].str() == b1[vs].str());
    CPPUNIT_ASSERT(b1["1 2"].str() == b1[vi].str());
    CPPUNIT_ASSERT(cb1["1 2"].str() == cb1[vs].str());
    CPPUNIT_ASSERT(cb1["1 2"].str() == cb1[vi].str());

    CPPUNIT_ASSERT(b1.at("1","2").str() == b1[vs].str());
    CPPUNIT_ASSERT(b1.at(1,2).str() == b1[vs].str());
    CPPUNIT_ASSERT(cb1.at("1","2").str() == cb1[vs].str());
    CPPUNIT_ASSERT(cb1.at(1,2).str() == cb1[vs].str());

    CPPUNIT_ASSERT(b1.front().str() == cb1.front().str());
    CPPUNIT_ASSERT(b1.back().str() == cb1.back().str());
  }

  void testModifiers()
  {
    SLHABlock b1("test");
    b1[""] = " 1 1 # 11";
    b1[""] = " 1 2 # 12";
    b1[""] = " 2 1 # 21";
    const SLHABlock cb1 = b1;

    b1.clear();
    CPPUNIT_ASSERT(b1.empty() == true);
    CPPUNIT_ASSERT(b1.size() == 0);
    CPPUNIT_ASSERT(b1.str() == "");

    vector<string> vs1;
    vs1.push_back("1");
    vs1.push_back("2");
    b1 = cb1;
    b1.erase(b1.find(vs1));
    CPPUNIT_ASSERT(b1.size() == 2);
    CPPUNIT_ASSERT(b1.str() == " 1 1 # 11\n 2 1 # 21\n");

    vector<string> vs2;
    vs2.push_back("2");
    vs2.push_back("1");
    b1 = cb1;
    b1.erase(b1.find(vs1), ++b1.find(vs2));
    CPPUNIT_ASSERT(b1.str() == " 1 1 # 11\n");

    b1.name("test2");
    CPPUNIT_ASSERT(b1.name() == "test2");

    b1 = cb1;
    b1.pop_back();
    CPPUNIT_ASSERT(b1.size() == 2);
    CPPUNIT_ASSERT(b1.str() == " 1 1 # 11\n 1 2 # 12\n");
    b1.push_back(" 2 1 # 21");
    CPPUNIT_ASSERT(b1.str() == cb1.str());

    b1.clear();
    CPPUNIT_ASSERT(b1.str() == "");
    b1.str(" 1 1 # 11\n 1 2 # 12\n 2 1 # 21");
    CPPUNIT_ASSERT(b1.str() == cb1.str());
  }

  void testIterators()
  {
    SLHABlock b1("test");
    b1[""] = " 1 1 # 1 1";
    b1[""] = " 1 2 # 1 2";
    b1[""] = " 2 1 # 2 1";
    b1[""] = " 2 2 # 2 2";
    const SLHABlock cb1 = b1;

    CPPUNIT_ASSERT((*b1.begin()).str() == (*cb1.begin()).str());
    CPPUNIT_ASSERT((*(b1.end()-1)).str() == (*(cb1.end()-1)).str());
    CPPUNIT_ASSERT((*b1.rbegin()).str() == (*cb1.rbegin()).str());
    CPPUNIT_ASSERT((*(b1.rend()-1)).str() == (*(cb1.rend()-1)).str());

    CPPUNIT_ASSERT(b1.begin()+0 == b1.end()-4);
    CPPUNIT_ASSERT(b1.begin()+1 == b1.end()-3);
    CPPUNIT_ASSERT(b1.begin()+2 == b1.end()-2);
    CPPUNIT_ASSERT(b1.begin()+3 == b1.end()-1);

    CPPUNIT_ASSERT(b1.rbegin()+0 == b1.rend()-4);
    CPPUNIT_ASSERT(b1.rbegin()+1 == b1.rend()-3);
    CPPUNIT_ASSERT(b1.rbegin()+2 == b1.rend()-2);
    CPPUNIT_ASSERT(b1.rbegin()+3 == b1.rend()-1);

    vector<string> vs1;
    vs1.push_back("1");
    vs1.push_back("1");

    vector<string> vs2;
    vs2.push_back("2");
    vs2.push_back("2");

    CPPUNIT_ASSERT((*b1.begin()).str() == (*b1.find(vs1)).str());
    CPPUNIT_ASSERT((*(b1.end()-1)).str() == (*b1.find(vs2)).str());
    CPPUNIT_ASSERT((*b1.rbegin()).str() == (*b1.find(vs2)).str());
    CPPUNIT_ASSERT((*(b1.rend()-1)).str() == (*b1.find(vs1)).str());

    SLHABlock::iterator last_but_one = b1.end()-2;
    b1.erase(b1.end()-1);
    CPPUNIT_ASSERT(b1.end()-1 == last_but_one);
    CPPUNIT_ASSERT(b1.begin()+2 == last_but_one);
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

} // namespace SLHAea

#endif // SLHAEA_TESTSLHABLOCK_H

// vim: sw=2 tw=78
