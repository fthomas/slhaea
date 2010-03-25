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

#include <sstream>
#include <string>
#include "slhaea.h"

using namespace std;
using namespace SLHAea;

BOOST_AUTO_TEST_SUITE(TestSLHA)

BOOST_AUTO_TEST_CASE(testAccessors)
{
  string test =
    "BLOCK test1 # 1st comment\n"
    " 1  1  # 2nd comment\n"
    " 2  1  # 3rd comment\n"
    "Block test2 # 4th comment\n"
    " 2  1  # 5th comment\n"
    " 3  2  # 6th comment\n"
    " 4  3  # 7th comment\n";

  SLHA s1; s1.str(test);
  const SLHA cs1 = s1;

  BOOST_CHECK(s1["test1"].size() == 3);
  BOOST_CHECK(cs1.at("test1").size() == 3);
  BOOST_CHECK(s1.front().size() == 3);
  BOOST_CHECK(cs1.front().size() == 3);

  BOOST_CHECK(s1["test2"].size() == 4);
  BOOST_CHECK(cs1.at("test2").size() == 4);
  BOOST_CHECK(s1.back().size() == 4);
  BOOST_CHECK(cs1.back().size() == 4);

  BOOST_CHECK(s1.str() == test);
  BOOST_CHECK(cs1.str() == test);

  BOOST_CHECK(cs1.field(SLHAKey("test2;4,3;1")) == "3");
  BOOST_CHECK(cs1.field(SLHAKey("test1;2;2")) == "# 3rd comment");
  BOOST_CHECK(cs1.field(string("test1;2;2")) == "# 3rd comment");

  BOOST_CHECK(s1.field(SLHAKey("test2;4,3;1")) == "3");
  BOOST_CHECK(s1.field(SLHAKey("test1;2;2")) == "# 3rd comment");
  BOOST_CHECK(s1.field(string("test1;2;2")) == "# 3rd comment");

  s1.field(SLHAKey("test2;4,3;1")) = "3.14";
  BOOST_CHECK(s1.field(SLHAKey("test2;4;1")) == "3.14");
}

BOOST_AUTO_TEST_CASE(testModifiers)
{
  string test =
    "BLOCK test1 # 1st comment\n"
    " 1  1  # 2nd comment\n"
    " 2  1  # 3rd comment\n"
    "Block test2 # 4th comment\n"
    " 2  1  # 5th comment\n"
    " 3  2  # 6th comment\n"
    " 4  3  # 7th comment\n";

  SLHA s1; s1.str(test);
  const SLHA cs1 = s1;

  s1["test1"][""] = " 3 1 # comment";
  BOOST_CHECK(s1["test1"].size() == 4);
  BOOST_CHECK(s1["test1"].back().str() == " 3 1 # comment");

  s1.clear();
  BOOST_CHECK(s1.size() == 0);
  BOOST_CHECK(s1.str() == "");

  s1 = cs1;
  s1.erase(s1.end()-1);
  BOOST_CHECK(s1.size() == 1);
  BOOST_CHECK(s1.str() ==
    "BLOCK test1 # 1st comment\n"
    " 1  1  # 2nd comment\n"
    " 2  1  # 3rd comment\n");

  s1 = cs1;
  s1.erase(s1.begin(), s1.end());
  BOOST_CHECK(s1.size() == 0);
  BOOST_CHECK(s1.str() == "");

  s1 = cs1;
  s1.pop_back();
  BOOST_CHECK(s1.size() == 1);
  BOOST_CHECK(s1.str() ==
    "BLOCK test1 # 1st comment\n"
    " 1  1  # 2nd comment\n"
    " 2  1  # 3rd comment\n");

  s1.push_back(cs1.at("test2"));
  BOOST_CHECK(s1.size() == 2);
  BOOST_CHECK(s1.str() == cs1.str());
}

BOOST_AUTO_TEST_CASE(testIterators)
{
  string test =
    "BLOCK test1\n"
    " 1  1\n"
    " 1  2\n"
    "Block test2\n"
    " 2  1\n"
    " 2  2\n"
    "bLoCk test3\n"
    " 3  1\n"
    " 3  2\n";

  SLHA s1; s1.str(test);
  const SLHA cs1 = s1;

  BOOST_CHECK((*s1.begin()).str() == (*cs1.begin()).str());
  BOOST_CHECK((*(s1.end()-1)).str() == (*(cs1.end()-1)).str());
  BOOST_CHECK((*s1.rbegin()).str() == (*cs1.rbegin()).str());
  BOOST_CHECK((*(s1.rend()-1)).str() == (*(cs1.rend()-1)).str());

  BOOST_CHECK(s1.begin()+0 == s1.end()-3);
  BOOST_CHECK(s1.begin()+1 == s1.end()-2);
  BOOST_CHECK(s1.begin()+2 == s1.end()-1);

  BOOST_CHECK(s1.rbegin()+0 == s1.rend()-3);
  BOOST_CHECK(s1.rbegin()+1 == s1.rend()-2);
  BOOST_CHECK(s1.rbegin()+2 == s1.rend()-1);

  BOOST_CHECK(cs1.begin()+0 == cs1.end()-3);
  BOOST_CHECK(cs1.begin()+1 == cs1.end()-2);
  BOOST_CHECK(cs1.begin()+2 == cs1.end()-1);

  BOOST_CHECK(cs1.rbegin()+0 == cs1.rend()-3);
  BOOST_CHECK(cs1.rbegin()+1 == cs1.rend()-2);
  BOOST_CHECK(cs1.rbegin()+2 == cs1.rend()-1);

  SLHA::iterator it = s1.erase(s1.end()-1);
  BOOST_CHECK(s1.end() == it);

  s1 = cs1;
  it = s1.erase(s1.begin()+1, s1.end());
  BOOST_CHECK(s1.end() == it);

  s1 = cs1;
  BOOST_CHECK(s1.begin() == s1.find("test1"));
  BOOST_CHECK(s1.end()-1 == s1.find("test3"));
  BOOST_CHECK(cs1.begin() == cs1.find("test1"));
  BOOST_CHECK(cs1.end()-1 == cs1.find("test3"));

  BOOST_CHECK(cs1.count("test3") == 1);
  BOOST_CHECK(cs1.count("test4") == 0);
  BOOST_CHECK(s1.count("test3") == 1);
  BOOST_CHECK(s1.count("test4") == 0);
}

BOOST_AUTO_TEST_CASE(testMiscellaneous)
{
  std::string data =
    "# first comment\n"
    "# second comment \n"
    "BLOCK FIRST\n"
    " 1  1  9.1\n"
    " 1  2  9.2\n"
    " 1  3  9.3\n"
    " 2  10.0\n"
    "   \n \n\n"
    "# comment before second block\n"
    "BlOcK second blob # comment\n"
    "  1  11.0  # first\n"
    "  2  false # second\n"
    " -3  true  # third \n"
    "  0 A  2  true  # third \n"
    " -9000 B    3  more \n"
    "# comment after second block\n";

  SLHA s1; s1.str(data);
  BOOST_CHECK(s1[""].front().str() == "# first comment");
  BOOST_CHECK(s1[""].back().str() == "# second comment");
  BOOST_CHECK(s1["fIrSt"]["2 10.0"].str() == " 2  10.0");
  BOOST_CHECK(s1["first"].size() == 6);
  BOOST_CHECK(s1["seCONd"]["-3"][1] == "true");
  BOOST_CHECK(s1["second"].size() == 7);
  BOOST_CHECK(s1["second"].at("BlOcK")[2] == "blob");
}

BOOST_AUTO_TEST_CASE(testSLHAKey)
{
  vector<string> keys = split_string("1");
  SLHAKey k1("RVKAPPAIN", keys, 1);
  BOOST_CHECK(k1.str() == "RVKAPPAIN;1;1");

  k1.line.push_back("2");
  BOOST_CHECK(k1.str() == "RVKAPPAIN;1,2;1");

  k1.line.clear();
  BOOST_CHECK(k1.str() == "RVKAPPAIN;;1");

  k1.block = "";
  k1.field = 0;
  BOOST_CHECK(k1.str() == ";;0");
  BOOST_CHECK(k1.str("RVKAPPAIN;1,3;1").str() == "RVKAPPAIN;1,3;1");

  stringstream ss("");
  ss << k1;
  BOOST_CHECK(k1.str() == ss.str());
}

BOOST_AUTO_TEST_SUITE_END()

// vim: sw=2 tw=78
