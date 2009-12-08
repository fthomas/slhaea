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

#ifndef SLHAPP_TESTSLHA_H
#define SLHAPP_TESTSLHA_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <slhapp.h>

using namespace std;

namespace SLHApp {

class TestSLHA : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestSLHA);
  /*CPPUNIT_TEST(testSlha);
  CPPUNIT_TEST(testSlhaBlock);
  CPPUNIT_TEST(testSlhaLine);*/
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}

  void tearDown() {}

  /*void testSlha()
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

    Slha s1; s1.str(data);
    CPPUNIT_ASSERT(s1[""].front().str() == "# first comment");
    CPPUNIT_ASSERT(s1[""].back().str() == "# second comment");
    CPPUNIT_ASSERT(s1["fIrSt"]["2 10.0"].str() == " 2  10.0");
    CPPUNIT_ASSERT(s1["seCONd"]["-3"][1] == "true");
    CPPUNIT_ASSERT(s1["second"].size() == 7);
    CPPUNIT_ASSERT(s1["second"].at("BlOcK")[2] == "blob");
  }

  void testSlhaBlock()
  {
    SlhaBlock b1;
    CPPUNIT_ASSERT(b1.name() == "");
    b1.name("test");
    CPPUNIT_ASSERT(b1.name() == "test");
    CPPUNIT_ASSERT(b1.size() == 0);

    b1.push_back(SlhaLine(" 1 a b c"));
    CPPUNIT_ASSERT(b1.front().str() == " 1 a b c");
    CPPUNIT_ASSERT(b1.back().str() == " 1 a b c");

    b1.push_back(SlhaLine(" 2 a b"));
    CPPUNIT_ASSERT(b1.front().str() == " 1 a b c");
    CPPUNIT_ASSERT(b1.back().str() == " 2 a b");
    CPPUNIT_ASSERT(b1.str() == " 1 a b c\n 2 a b\n");

    b1[""] = " 2 a c";
    std::vector<int> vi;
    std::vector<std::string> vs;
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

  void testSlhaLine()
  {
    SlhaLine l1;
    CPPUNIT_ASSERT(l1.str() == "");
    
    l1.str(" 1 2 3 4 ");
    CPPUNIT_ASSERT(l1.str() == " 1 2 3 4");
    CPPUNIT_ASSERT(l1.size() == 4);
    
    l1 = "BLOCK TEST # comment";
    CPPUNIT_ASSERT(l1.str() == "BLOCK TEST # comment");
    CPPUNIT_ASSERT(l1[0] == "BLOCK");
    CPPUNIT_ASSERT(l1[1] == "TEST");
    CPPUNIT_ASSERT(l1[2] == "# comment");

    l1 = "BLOCK  TEST  # comment";
    CPPUNIT_ASSERT(l1.str() == "BLOCK  TEST  # comment");
    CPPUNIT_ASSERT(l1[0] == "BLOCK");
    CPPUNIT_ASSERT(l1[1] == "TEST");
    CPPUNIT_ASSERT(l1[2] == "# comment");

    l1 = "# one long comment with trailing spaces    ";
    CPPUNIT_ASSERT(l1[0] == "# one long comment with trailing spaces");
    CPPUNIT_ASSERT(l1.size() == 1);

    l1 = "one long data line with trailing spaces    ";
    CPPUNIT_ASSERT(l1.str() == "one long data line with trailing spaces");
    CPPUNIT_ASSERT(l1[6] == "spaces");
    CPPUNIT_ASSERT(l1.size() == 7);

    l1 = "Hello brave new \n world!";
    CPPUNIT_ASSERT(l1.str() == "Hello brave new");
    CPPUNIT_ASSERT(l1.size() == 3);

    l1 = "Hello stupid brave new world!";
    CPPUNIT_ASSERT(l1.str() == "Hello stupid brave new world!");
    l1[1] = "";
    CPPUNIT_ASSERT(l1.str() == "Hello        brave new world!");
    l1[1] = "dumb";
    CPPUNIT_ASSERT(l1.str() == "Hello dumb   brave new world!");
    l1[3] = "brand-new";
    CPPUNIT_ASSERT(l1.str() == "Hello dumb   brave brand-new world!");
    l1[0] = "Goodbye";
    CPPUNIT_ASSERT(l1.str() == "Goodbye dumb brave brand-new world!");

    l1 = " 23  1.123456789E+999  # some arcane value";
    CPPUNIT_ASSERT(l1[0] == "23");
    CPPUNIT_ASSERT(l1[1] == "1.123456789E+999");
    CPPUNIT_ASSERT(l1[2] == "# some arcane value");

    l1 = "\n 1 2 3 4 5";
    CPPUNIT_ASSERT(l1.empty() == true);
    CPPUNIT_ASSERT(l1.size() == 1);
    l1 = "  \n 1 2 3 4 5  ";
    CPPUNIT_ASSERT(l1.empty() == true);
    CPPUNIT_ASSERT(l1.size() == 1);
    l1 = "";
    CPPUNIT_ASSERT(l1.empty() == true);
    CPPUNIT_ASSERT(l1.size() == 1);
    l1 = "   ";
    CPPUNIT_ASSERT(l1.empty() == true);
    CPPUNIT_ASSERT(l1.size() == 1);
    l1 = " . ";
    CPPUNIT_ASSERT(l1.empty() == false);
    CPPUNIT_ASSERT(l1.size() == 1);

    l1 = " 1 22 333 4444 ";
    l1.append(" 55555 # a comment ");
    CPPUNIT_ASSERT(l1.size() == 6);
    CPPUNIT_ASSERT(l1[3] == "4444");
    CPPUNIT_ASSERT(l1[4] == "55555");
    CPPUNIT_ASSERT(l1[5] == "# a comment");
    l1 += " 7 ";
    CPPUNIT_ASSERT(l1[5] == "# a comment 7");

    l1 = " test  str() and   strPlain() ";
    CPPUNIT_ASSERT(l1.str() == " test  str() and   strPlain()");
    CPPUNIT_ASSERT(l1.strPlain() == "test str() and strPlain()");

    l1 = " 1 2 3 4 5 ";
    CPPUNIT_ASSERT(l1.back() == "5");
    l1.back() = "6";
    CPPUNIT_ASSERT(l1.back() == "6");
    CPPUNIT_ASSERT(l1.front() == "1");
    l1.front() = "-1";
    CPPUNIT_ASSERT(l1.front() == "-1");

    CPPUNIT_ASSERT(l1.front() == *l1.begin());
    CPPUNIT_ASSERT(l1.back() == *(l1.end()-1));
    CPPUNIT_ASSERT(*(l1.end()-2) == "4");
    *l1.begin() = "0";
    CPPUNIT_ASSERT(l1.front() == "0");
    *(l1.end()-1) = "7";
    CPPUNIT_ASSERT(l1.back() == "7");

    CPPUNIT_ASSERT(*l1.rbegin() == l1.back());
    CPPUNIT_ASSERT(*(l1.rend()-1) == l1.front());
    *l1.rbegin() = "8";
    CPPUNIT_ASSERT(l1.back() == "8");
    *(l1.rend()-1) = "-2";
    CPPUNIT_ASSERT(l1.front() == "-2");
  }*/
};

} // namespace SLHApp

#endif // SLHAPP_TESTSLHA_H

// vim: sw=2 tw=78
