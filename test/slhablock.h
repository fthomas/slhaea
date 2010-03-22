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

#include <string>
#include <vector>
#include "slhaea.h"

using namespace std;
using namespace SLHAea;

BOOST_AUTO_TEST_SUITE(TestSLHABlock)

BOOST_AUTO_TEST_CASE(testConstructors)
{
  SLHABlock b1, b2("test");
  BOOST_CHECK(b1.name() == "");
  BOOST_CHECK(b2.name() == "test");

  b1.name("test");
  b2.name("");
  BOOST_CHECK(b1.name() == "test");
  BOOST_CHECK(b2.name() == "");
}

BOOST_AUTO_TEST_CASE(testAccessors)
{
  SLHABlock b1("test");
  b1[""] = " 1 1 # 11";
  b1[""] = " 1 2 # 12";
  b1[""] = " 2 1 # 21";
  const SLHABlock cb1 = b1;

  string out = " 1 1 # 11\n"
               " 1 2 # 12\n"
               " 2 1 # 21\n";
  BOOST_CHECK(b1.str() == out);

  BOOST_CHECK(b1["1 2"][2] == "# 12");
  BOOST_CHECK(b1.front().str() == " 1 1 # 11");
  BOOST_CHECK(b1.back().str() == " 2 1 # 21");
  BOOST_CHECK(b1.back().str() == b1.at(2,1).str());

  BOOST_CHECK(cb1.at("1", "2")[2] == "# 12");
  BOOST_CHECK(cb1.front().str() == " 1 1 # 11");
  BOOST_CHECK(cb1.back().str() == " 2 1 # 21");
  BOOST_CHECK(cb1.back().str() == cb1.at(2,1).str());

  vector<string> vs;
  vector<int> vi;
  vs.push_back("1");
  vs.push_back("2");
  vi.push_back(1);
  vi.push_back(2);
  BOOST_CHECK(b1["1 2"].str() == b1[vs].str());
  BOOST_CHECK(b1["1 2"].str() == b1[vi].str());
  BOOST_CHECK(cb1.at("1", "2").str() == cb1.at(vs).str());
  BOOST_CHECK(cb1.at("1", "2").str() == cb1.at(vi).str());

  BOOST_CHECK(b1.at("1","2").str() == b1[vs].str());
  BOOST_CHECK(b1.at(1,2).str() == b1[vs].str());
  BOOST_CHECK(cb1.at("1","2").str() == cb1.at(vs).str());
  BOOST_CHECK(cb1.at(1,2).str() == cb1.at(vs).str());

  BOOST_CHECK(b1.front().str() == cb1.front().str());
  BOOST_CHECK(b1.back().str() == cb1.back().str());

  BOOST_CHECK(b1[1].str() == b1.front().str());
  BOOST_CHECK(cb1.at(1).str() == cb1.front().str());
  BOOST_CHECK(b1[2].str() == b1.back().str());
  BOOST_CHECK(cb1.at(2).str() == cb1.back().str());
}

BOOST_AUTO_TEST_CASE(testModifiers)
{
  SLHABlock b1("test");
  b1[""] = " 1 1 # 11";
  b1[""] = " 1 2 # 12";
  b1[""] = " 2 1 # 21";
  const SLHABlock cb1 = b1;

  b1.clear();
  BOOST_CHECK(b1.empty() == true);
  BOOST_CHECK(b1.size() == 0);
  BOOST_CHECK(b1.str() == "");

  vector<string> vs1;
  vs1.push_back("1");
  vs1.push_back("2");
  b1 = cb1;
  b1.erase(b1.find(vs1));
  BOOST_CHECK(b1.size() == 2);
  BOOST_CHECK(b1.str() == " 1 1 # 11\n 2 1 # 21\n");

  vector<string> vs2;
  vs2.push_back("2");
  vs2.push_back("1");
  b1 = cb1;
  b1.erase(b1.find(vs1), ++b1.find(vs2));
  BOOST_CHECK(b1.str() == " 1 1 # 11\n");

  b1.name("test2");
  BOOST_CHECK(b1.name() == "test2");

  b1 = cb1;
  b1.pop_back();
  BOOST_CHECK(b1.size() == 2);
  BOOST_CHECK(b1.str() == " 1 1 # 11\n 1 2 # 12\n");
  b1.push_back(" 2 1 # 21");
  BOOST_CHECK(b1.str() == cb1.str());

  b1.clear();
  BOOST_CHECK(b1.str() == "");
  b1.str(" 1 1 # 11\n 1 2 # 12\n 2 1 # 21");
  BOOST_CHECK(b1.str() == cb1.str());
}

BOOST_AUTO_TEST_CASE(testIterators)
{
  SLHABlock b1("test");
  b1[""] = " 1 1 # 1 1";
  b1[""] = " 1 2 # 1 2";
  b1[""] = " 2 1 # 2 1";
  b1[""] = " 2 2 # 2 2";
  const SLHABlock cb1 = b1;

  BOOST_CHECK((*b1.begin()).str() == (*cb1.begin()).str());
  BOOST_CHECK((*(b1.end()-1)).str() == (*(cb1.end()-1)).str());
  BOOST_CHECK((*b1.rbegin()).str() == (*cb1.rbegin()).str());
  BOOST_CHECK((*(b1.rend()-1)).str() == (*(cb1.rend()-1)).str());

  BOOST_CHECK(b1.begin()+0 == b1.end()-4);
  BOOST_CHECK(b1.begin()+1 == b1.end()-3);
  BOOST_CHECK(b1.begin()+2 == b1.end()-2);
  BOOST_CHECK(b1.begin()+3 == b1.end()-1);

  BOOST_CHECK(b1.rbegin()+0 == b1.rend()-4);
  BOOST_CHECK(b1.rbegin()+1 == b1.rend()-3);
  BOOST_CHECK(b1.rbegin()+2 == b1.rend()-2);
  BOOST_CHECK(b1.rbegin()+3 == b1.rend()-1);

  vector<string> vs1;
  vs1.push_back("1");
  vs1.push_back("1");

  vector<string> vs2;
  vs2.push_back("2");
  vs2.push_back("2");

  BOOST_CHECK((*b1.begin()).str() == (*b1.find(vs1)).str());
  BOOST_CHECK((*(b1.end()-1)).str() == (*b1.find(vs2)).str());
  BOOST_CHECK((*b1.rbegin()).str() == (*b1.find(vs2)).str());
  BOOST_CHECK((*(b1.rend()-1)).str() == (*b1.find(vs1)).str());

  SLHABlock::iterator last_but_one = b1.end()-2;
  b1.erase(b1.end()-1);
  BOOST_CHECK(b1.end()-1 == last_but_one);
  BOOST_CHECK(b1.begin()+2 == last_but_one);
}

BOOST_AUTO_TEST_CASE(testMiscellaneous)
{
  SLHABlock b1;
  BOOST_CHECK(b1.name() == "");
  b1.name("test");
  BOOST_CHECK(b1.name() == "test");
  BOOST_CHECK(b1.size() == 0);

  b1.push_back(SLHALine(" 1 a b c"));
  BOOST_CHECK(b1.front().str() == " 1 a b c");
  BOOST_CHECK(b1.back().str() == " 1 a b c");

  b1.push_back(SLHALine(" 2 a b"));
  BOOST_CHECK(b1.front().str() == " 1 a b c");
  BOOST_CHECK(b1.back().str() == " 2 a b");
  BOOST_CHECK(b1.str() == " 1 a b c\n 2 a b\n");

  b1[""] = " 2 a c";
  vector<int> vi;
  vector<string> vs;
  vi.push_back(1);
  vs.push_back("1");
  BOOST_CHECK(b1[vi].str() == " 1 a b c");
  BOOST_CHECK(b1[vs].str() == " 1 a b c");
  BOOST_CHECK(b1["2 a"].str() == " 2 a b");
  BOOST_CHECK(b1["2 a c"].str() == " 2 a c");
  BOOST_CHECK(b1["2 a c"][2] == "c");
  BOOST_CHECK(b1.at("2", "a", "c")[2] == "c");
  b1[""] = " 3 -1 y zzz";
  BOOST_CHECK(b1.at("3").str() == " 3 -1 y zzz");
  BOOST_CHECK(b1.at(3,-1).str() == " 3 -1 y zzz");
  b1.push_back(" 4 1 f g");
  BOOST_CHECK(b1.back().str() == " 4 1 f g");

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
  BOOST_CHECK(b1.size() == 4);
  BOOST_CHECK(b1["3 2"][2] == "gg");
  BOOST_CHECK(b1.str() == block_bh);
}

BOOST_AUTO_TEST_SUITE_END()

// vim: sw=2 tw=78
