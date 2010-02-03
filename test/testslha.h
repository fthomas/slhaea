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

#ifndef SLHAEA_TESTSLHA_H
#define SLHAEA_TESTSLHA_H

#include <sstream>
#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <slhaea.h>

using namespace std;

namespace SLHAea {

class TestSLHA : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestSLHA);
  CPPUNIT_TEST(testAccessors);
  CPPUNIT_TEST(testModifiers);
  CPPUNIT_TEST(testIterators);
  CPPUNIT_TEST(testMiscellaneous);
  CPPUNIT_TEST(testSLHAKey);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}

  void tearDown() {}

  void testAccessors()
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

    CPPUNIT_ASSERT(s1["test1"].size() == 3);
    CPPUNIT_ASSERT(cs1.at("test1").size() == 3);
    CPPUNIT_ASSERT(s1.front().size() == 3);
    CPPUNIT_ASSERT(cs1.front().size() == 3);

    CPPUNIT_ASSERT(s1["test2"].size() == 4);
    CPPUNIT_ASSERT(cs1.at("test2").size() == 4);
    CPPUNIT_ASSERT(s1.back().size() == 4);
    CPPUNIT_ASSERT(cs1.back().size() == 4);

    CPPUNIT_ASSERT(s1.str() == test);
    CPPUNIT_ASSERT(cs1.str() == test);

    CPPUNIT_ASSERT(cs1.field(SLHAKey("test2;4,3;1")) == "3");
    CPPUNIT_ASSERT(cs1.field(SLHAKey("test1;2;2")) == "# 3rd comment");
    CPPUNIT_ASSERT(cs1.field(string("test1;2;2")) == "# 3rd comment");

    CPPUNIT_ASSERT(s1.field(SLHAKey("test2;4,3;1")) == "3");
    CPPUNIT_ASSERT(s1.field(SLHAKey("test1;2;2")) == "# 3rd comment");
    CPPUNIT_ASSERT(s1.field(string("test1;2;2")) == "# 3rd comment");

    s1.field(SLHAKey("test2;4,3;1")) = "3.14";
    CPPUNIT_ASSERT(s1.field(SLHAKey("test2;4;1")) == "3.14");
  }

  void testModifiers()
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
    CPPUNIT_ASSERT(s1["test1"].size() == 4);
    CPPUNIT_ASSERT(s1["test1"].back().str() == " 3 1 # comment");

    s1.clear();
    CPPUNIT_ASSERT(s1.size() == 0);
    CPPUNIT_ASSERT(s1.str() == "");

    s1 = cs1;
    s1.erase(s1.end()-1);
    CPPUNIT_ASSERT(s1.size() == 1);
    CPPUNIT_ASSERT(s1.str() ==
      "BLOCK test1 # 1st comment\n"
      " 1  1  # 2nd comment\n"
      " 2  1  # 3rd comment\n");

    s1 = cs1;
    s1.erase(s1.begin(), s1.end());
    CPPUNIT_ASSERT(s1.size() == 0);
    CPPUNIT_ASSERT(s1.str() == "");

    s1 = cs1;
    s1.pop_back();
    CPPUNIT_ASSERT(s1.size() == 1);
    CPPUNIT_ASSERT(s1.str() ==
      "BLOCK test1 # 1st comment\n"
      " 1  1  # 2nd comment\n"
      " 2  1  # 3rd comment\n");

    s1.push_back(cs1.at("test2"));
    CPPUNIT_ASSERT(s1.size() == 2);
    CPPUNIT_ASSERT(s1.str() == cs1.str());
  }

  void testIterators()
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

    CPPUNIT_ASSERT((*s1.begin()).str() == (*cs1.begin()).str());
    CPPUNIT_ASSERT((*(s1.end()-1)).str() == (*(cs1.end()-1)).str());
    CPPUNIT_ASSERT((*s1.rbegin()).str() == (*cs1.rbegin()).str());
    CPPUNIT_ASSERT((*(s1.rend()-1)).str() == (*(cs1.rend()-1)).str());

    CPPUNIT_ASSERT(s1.begin()+0 == s1.end()-3);
    CPPUNIT_ASSERT(s1.begin()+1 == s1.end()-2);
    CPPUNIT_ASSERT(s1.begin()+2 == s1.end()-1);

    CPPUNIT_ASSERT(s1.rbegin()+0 == s1.rend()-3);
    CPPUNIT_ASSERT(s1.rbegin()+1 == s1.rend()-2);
    CPPUNIT_ASSERT(s1.rbegin()+2 == s1.rend()-1);

    CPPUNIT_ASSERT(cs1.begin()+0 == cs1.end()-3);
    CPPUNIT_ASSERT(cs1.begin()+1 == cs1.end()-2);
    CPPUNIT_ASSERT(cs1.begin()+2 == cs1.end()-1);

    CPPUNIT_ASSERT(cs1.rbegin()+0 == cs1.rend()-3);
    CPPUNIT_ASSERT(cs1.rbegin()+1 == cs1.rend()-2);
    CPPUNIT_ASSERT(cs1.rbegin()+2 == cs1.rend()-1);

    SLHA::iterator it = s1.erase(s1.end()-1);
    CPPUNIT_ASSERT(s1.end() == it);

    s1 = cs1;
    it = s1.erase(s1.begin()+1, s1.end());
    CPPUNIT_ASSERT(s1.end() == it);

    s1 = cs1;
    CPPUNIT_ASSERT(s1.begin() == s1.find("test1"));
    CPPUNIT_ASSERT(s1.end()-1 == s1.find("test3"));
    CPPUNIT_ASSERT(cs1.begin() == cs1.find("test1"));
    CPPUNIT_ASSERT(cs1.end()-1 == cs1.find("test3"));

    CPPUNIT_ASSERT(cs1.count("test3") == 1);
    CPPUNIT_ASSERT(cs1.count("test4") == 0);
    CPPUNIT_ASSERT(s1.count("test3") == 1);
    CPPUNIT_ASSERT(s1.count("test4") == 0);
  }

  void testMiscellaneous()
  {
    std::string data =
      "# first comment\n"
      "# second comment \n"
      "BLOCK FIRST\n"
      " 1  1  9.1\n"
      " 1  2  9.2\n"
      " 1  3  9.3\n"
      " 2  10.0\n"
      "# comment before second block\n"
      "BlOcK second blob # comment\n"
      "  1  11.0  # first\n"
      "  2  false # second\n"
      " -3  true  # third \n"
      "  0 A  2  true  # third \n"
      " -9000 B    3  more \n"
      "# comment after second block\n";

    SLHA s1; s1.str(data);
    CPPUNIT_ASSERT(s1[""].front().str() == "# first comment");
    CPPUNIT_ASSERT(s1[""].back().str() == "# second comment");
    CPPUNIT_ASSERT(s1["fIrSt"]["2 10.0"].str() == " 2  10.0");
    CPPUNIT_ASSERT(s1["seCONd"]["-3"][1] == "true");
    CPPUNIT_ASSERT(s1["second"].size() == 7);
    CPPUNIT_ASSERT(s1["second"].at("BlOcK")[2] == "blob");
  }

  void testSLHAKey()
  {
    vector<string> keys = to_string_vector("1");
    SLHAKey k1("RVKAPPAIN", keys, 1);
    CPPUNIT_ASSERT(k1.str() == "RVKAPPAIN;1;1");

    k1.line.push_back("2");
    CPPUNIT_ASSERT(k1.str() == "RVKAPPAIN;1,2;1");

    k1.line.clear();
    CPPUNIT_ASSERT(k1.str() == "RVKAPPAIN;;1");

    k1.block = "";
    k1.field = 0;
    CPPUNIT_ASSERT(k1.str() == ";;0");
    CPPUNIT_ASSERT(k1.str("RVKAPPAIN;1,3;1").str() == "RVKAPPAIN;1,3;1");

    stringstream ss("");
    ss << k1;
    CPPUNIT_ASSERT(k1.str() == ss.str());
  }
};

} // namespace SLHAea

#endif // SLHAEA_TESTSLHA_H

// vim: sw=2 tw=78
