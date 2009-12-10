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

#ifndef SLHAEA_TESTSLHA_H
#define SLHAEA_TESTSLHA_H

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
  //CPPUNIT_TEST(testIterators);
  CPPUNIT_TEST(testFileOperations);
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
    CPPUNIT_ASSERT(cs1["test1"].size() == 3);
    CPPUNIT_ASSERT(s1.front().size() == 3);
    CPPUNIT_ASSERT(cs1.front().size() == 3);

    CPPUNIT_ASSERT(s1["test2"].size() == 4);
    CPPUNIT_ASSERT(cs1["test2"].size() == 4);
    CPPUNIT_ASSERT(s1.back().size() == 4);
    CPPUNIT_ASSERT(cs1.back().size() == 4);

    CPPUNIT_ASSERT(s1.str() == test);
    CPPUNIT_ASSERT(cs1.str() == test);
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

    s1.push_back(cs1["test2"]);
    CPPUNIT_ASSERT(s1.size() == 2);
    CPPUNIT_ASSERT(s1.str() == cs1.str());
  }

  void testFileOperations()
  {
    string test =
      "BLOCK test1 # 1st comment\n"
      " 1  1  # 2nd comment\n"
      " 2  1  # 3rd comment\n"
      "Block test2 # 4th comment\n"
      " 2  1  # 5th comment\n"
      "3  2  # 6th comment\n"
      " 4  3  # 7th comment\n";

    SLHA s1; s1.str(test);
    s1.write_file(".slhaea_test");
    const SLHA cs1(".slhaea_test");
    CPPUNIT_ASSERT(cs1.str() == s1.str());
    CPPUNIT_ASSERT(cs1.str() == test);
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
    k1.element = 0;
    CPPUNIT_ASSERT(k1.str() == ";;0");
    CPPUNIT_ASSERT(k1.str("RVKAPPAIN;1,3;1").str() == "RVKAPPAIN;1,3;1");
  }
};

} // namespace SLHAea

#endif // SLHAEA_TESTSLHA_H

// vim: sw=2 tw=78
