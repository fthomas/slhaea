// SLHAea - another SUSY Les Houches Accord input/output library
// Copyright © 2009-2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file ../LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>
#include <string>
#include "slhaea.h"

using namespace std;
using namespace SLHAea;

BOOST_AUTO_TEST_SUITE(TestColl)

struct F {
  F() {
    s1 = "BLOCK test1 # 1st comment\n"
         " 1  1       # 2nd comment\n"
         " 2  1       # 3rd comment\n"
         "Block test2 # 4th comment\n"
         " 2  1       # 5th comment\n"
         " 3  2       # 6th comment\n"
         " 4  3       # 7th comment\n";

    s2 = "BLOCK test1\n"
         " 1  1\n"
         " 1  2\n"
         "Block test2\n"
         " 2  1\n"
         " 2  2\n"
         "bLoCk test3\n"
         " 3  1\n"
         " 3  2\n"
         "BlOcK test4\n"
         " 4  1\n"
         " 4  2\n";
  }

  ~F() {}

  string s1, s2;
};

BOOST_FIXTURE_TEST_CASE(testReadWrite, F)
{
  stringstream ss1(s1);
  stringstream ss2(s1);

  Coll c1, c2, c3;
  c1.read(ss1);
  c2.str(s1);
  ss2 >> c3;

  BOOST_CHECK(c1 == c2);
  BOOST_CHECK(c1 == c3);
  BOOST_CHECK(c2 == c3);

  BOOST_CHECK(c1.str() == s1);
  BOOST_CHECK(c2.str() == s1);
  BOOST_CHECK(c3.str() == s1);
}

BOOST_FIXTURE_TEST_CASE(testField, F)
{
  Coll c1;
  c1.str(s1);
  const Coll cc1 = c1;

  BOOST_CHECK(c1.field(Key("TEST2;4,3;1"))   == "3");
  BOOST_CHECK(c1.field(Key("test1;2;2"))     == "# 3rd comment");
  BOOST_CHECK(c1.field(string("tEsT1;2;2"))  == "# 3rd comment");
  BOOST_CHECK(c1.field("tEsT1;2;2")          == "# 3rd comment");

  BOOST_CHECK(cc1.field(Key("test2;4,3;1"))  == "3");
  BOOST_CHECK(cc1.field(Key("test1;2;2"))    == "# 3rd comment");
  BOOST_CHECK(cc1.field(string("test1;2;2")) == "# 3rd comment");
  BOOST_CHECK(cc1.field("test1;2;2")         == "# 3rd comment");

  c1.field(Key("test2;4,3;1")) = "3.14";

  BOOST_CHECK(c1.field(Key("test2;4;1"))    == "3.14");
  BOOST_CHECK(c1.field(string("test2;4;1")) == "3.14");
  BOOST_CHECK(c1.field("test2;4;1")         == "3.14");

  c1.field("test2;4;1") = "4.15";

  BOOST_CHECK(c1.field(Key("test2;4;1"))    == "4.15");
  BOOST_CHECK(c1.field(string("test2;4;1")) == "4.15");
  BOOST_CHECK(c1.field("test2;4;1")         == "4.15");
}

BOOST_FIXTURE_TEST_CASE(testSubscriptAccessor, F)
{
  stringstream ss1(s1);
  Coll c1(ss1);

  BOOST_CHECK(c1["test1"].size() == 3);
  BOOST_CHECK(c1["test2"].size() == 4);

  BOOST_CHECK(c1["test1"].name() == "test1");
  BOOST_CHECK(c1["test2"].name() == "test2");

  BOOST_CHECK(c1["tEsT1"].front().is_block_def() == true);
  BOOST_CHECK(c1["TeSt2"].front().is_block_def() == true);

  c1["test3"];

  BOOST_CHECK(c1["test3"].size()  == 0);
  BOOST_CHECK(c1["test3"].empty() == true);

  c1["test3"][""] = "BLOCK test3";

  BOOST_CHECK(c1["TEST3"].size() == 1);
  BOOST_CHECK(c1["TeSt3"].name() == "test3");
  BOOST_CHECK(c1["tEsT3"].front().is_block_def() == true);
  BOOST_CHECK(c1["test3"].back().is_block_def()  == true);
}

BOOST_FIXTURE_TEST_CASE(testAccessors, F)
{
  stringstream ss1(s1);
  Coll c1(ss1);
  const Coll cc1 = c1;

  BOOST_CHECK(c1.at("test1").size()  == 3);
  BOOST_CHECK(c1.at("test2").size()  == 4);
  BOOST_CHECK(cc1.at("tESt1").size() == 3);
  BOOST_CHECK(cc1.at("tESt2").size() == 4);

  BOOST_CHECK(c1.at("test1").name()  == "test1");
  BOOST_CHECK(c1.at("test2").name()  == "test2");
  BOOST_CHECK(cc1.at("test1").name() == "test1");
  BOOST_CHECK(cc1.at("test2").name() == "test2");

  BOOST_CHECK(c1.at("test1").front().is_block_def()  == true);
  BOOST_CHECK(c1.at("test2").front().is_block_def()  == true);
  BOOST_CHECK(cc1.at("TEST1").front().is_block_def() == true);
  BOOST_CHECK(cc1.at("TEST2").front().is_block_def() == true);

  BOOST_CHECK(c1.at("test1").back().is_data_line()   == true);
  BOOST_CHECK(c1.at("test2").back().is_data_line()   == true);
  BOOST_CHECK(cc1.at("TesT1").back().is_data_line()  == true);
  BOOST_CHECK(cc1.at("TesT2").back().is_data_line()  == true);
}

BOOST_FIXTURE_TEST_CASE(testIterators, F)
{
  Coll c1;
  c1.str(s2);
  const Coll cc1 = c1;

  BOOST_CHECK((*(c1.begin()))  == (*(cc1.begin())));
  BOOST_CHECK((*(c1.end()-1))  == (*(cc1.end()-1)));
  BOOST_CHECK((*(c1.rbegin())) == (*(cc1.rbegin())));
  BOOST_CHECK((*(c1.rend()-1)) == (*(cc1.rend()-1)));

  BOOST_CHECK(c1.begin()+0 == c1.end()-4);
  BOOST_CHECK(c1.begin()+1 == c1.end()-3);
  BOOST_CHECK(c1.begin()+2 == c1.end()-2);
  BOOST_CHECK(c1.begin()+3 == c1.end()-1);

  BOOST_CHECK(c1.rbegin()+0 == c1.rend()-4);
  BOOST_CHECK(c1.rbegin()+1 == c1.rend()-3);
  BOOST_CHECK(c1.rbegin()+2 == c1.rend()-2);
  BOOST_CHECK(c1.rbegin()+3 == c1.rend()-1);

  BOOST_CHECK(c1.cbegin()+0 == c1.cend()-4);
  BOOST_CHECK(c1.cbegin()+1 == c1.cend()-3);
  BOOST_CHECK(c1.cbegin()+2 == c1.cend()-2);
  BOOST_CHECK(c1.cbegin()+3 == c1.cend()-1);

  BOOST_CHECK(c1.crbegin()+0 == c1.crend()-4);
  BOOST_CHECK(c1.crbegin()+1 == c1.crend()-3);
  BOOST_CHECK(c1.crbegin()+2 == c1.crend()-2);
  BOOST_CHECK(c1.crbegin()+3 == c1.crend()-1);

  BOOST_CHECK(cc1.begin()+0 == cc1.end()-4);
  BOOST_CHECK(cc1.begin()+1 == cc1.end()-3);
  BOOST_CHECK(cc1.begin()+2 == cc1.end()-2);
  BOOST_CHECK(cc1.begin()+3 == cc1.end()-1);

  BOOST_CHECK(cc1.rbegin()+0 == cc1.rend()-4);
  BOOST_CHECK(cc1.rbegin()+1 == cc1.rend()-3);
  BOOST_CHECK(cc1.rbegin()+2 == cc1.rend()-2);
  BOOST_CHECK(cc1.rbegin()+3 == cc1.rend()-1);

  BOOST_CHECK(cc1.cbegin()+0 == cc1.cend()-4);
  BOOST_CHECK(cc1.cbegin()+1 == cc1.cend()-3);
  BOOST_CHECK(cc1.cbegin()+2 == cc1.cend()-2);
  BOOST_CHECK(cc1.cbegin()+3 == cc1.cend()-1);

  BOOST_CHECK(cc1.crbegin()+0 == cc1.crend()-4);
  BOOST_CHECK(cc1.crbegin()+1 == cc1.crend()-3);
  BOOST_CHECK(cc1.crbegin()+2 == cc1.crend()-2);
  BOOST_CHECK(cc1.crbegin()+3 == cc1.crend()-1);

  BOOST_CHECK((*(c1.begin()+0)).name() == "test1");
  BOOST_CHECK((*(c1.begin()+1)).name() == "test2");
  BOOST_CHECK((*(c1.begin()+2)).name() == "test3");
  BOOST_CHECK((*(c1.begin()+3)).name() == "test4");
}

BOOST_FIXTURE_TEST_CASE(testIntrospection, F)
{
  Coll c1;
  c1.str(s2);
  const Coll cc1 = c1;

  BOOST_CHECK(c1.find("test1")  == c1.begin()+0);
  BOOST_CHECK(c1.find("test2")  == c1.begin()+1);
  BOOST_CHECK(c1.find("test3")  == c1.begin()+2);
  BOOST_CHECK(c1.find("test4")  == c1.end()-1);
  BOOST_CHECK(c1.find("test5")  == c1.end()-0);

  BOOST_CHECK(cc1.find("test1")  == cc1.begin()+0);
  BOOST_CHECK(cc1.find("test2")  == cc1.begin()+1);
  BOOST_CHECK(cc1.find("test3")  == cc1.begin()+2);
  BOOST_CHECK(cc1.find("test4")  == cc1.end()-1);
  BOOST_CHECK(cc1.find("test5")  == cc1.end()-0);

  BOOST_CHECK(c1.count("test1") == 1);
  BOOST_CHECK(c1.count("test5") == 0);

  c1["test2"].name("TEST1");
  c1["test3"].name("TEst1");

  BOOST_CHECK(c1.count("test1") == 3);
  BOOST_CHECK(c1.count("test2") == 0);
  BOOST_CHECK(c1.count("test3") == 0);

  BOOST_CHECK(c1.size()  == 4);
  BOOST_CHECK(cc1.size() == 4);

  c1.clear();

  BOOST_CHECK(c1.size()  == 0);
  BOOST_CHECK(cc1.size() == 4);
}

BOOST_FIXTURE_TEST_CASE(testPushPop, F)
{
  Coll c1;

  BOOST_CHECK(c1.size()  == 0);
  BOOST_CHECK(c1.empty() == true);

  c1.push_back("BLOCK test3");
  c1.push_back(Block("test4"));

  BOOST_CHECK(c1.size()  == 2);
  BOOST_CHECK(c1.empty() == false);

  BOOST_CHECK(c1.count("test3") == 1);
  BOOST_CHECK(c1.count("test4") == 1);

  c1.pop_back();

  BOOST_CHECK(c1.size()  == 1);
  BOOST_CHECK(c1.empty() == false);

  BOOST_CHECK(c1.count("test3") == 1);
  BOOST_CHECK(c1.count("test4") == 0);

  c1.pop_back();

  BOOST_CHECK(c1.size()  == 0);
  BOOST_CHECK(c1.empty() == true);

  BOOST_CHECK(c1.count("test3") == 0);
  BOOST_CHECK(c1.count("test4") == 0);
}

BOOST_FIXTURE_TEST_CASE(testInsertErase, F)
{
  Coll c1;
  c1.str(s1);
  const Coll cc1 = c1;

  BOOST_CHECK(c1.size() == 2);
  BOOST_CHECK(c1.count("test1") == 1);

  c1.insert(c1.begin(), c1.front());

  BOOST_CHECK(c1.size() == 3);
  BOOST_CHECK(c1.count("test1") == 2);

  c1.insert(c1.end(), c1.begin(), c1.begin()+2);

  BOOST_CHECK(c1.size() == 5);
  BOOST_CHECK(c1.count("test1") == 4);

  c1.back().name("test3");
  c1.erase("test3");

  BOOST_CHECK(c1.size() == 4);
  BOOST_CHECK(c1.count("test1") == 3);

  c1.erase(c1.end()-1, c1.end());

  BOOST_CHECK(c1.size() == 3);
  BOOST_CHECK(c1.count("test1") == 2);

  c1.erase(c1.begin()+1);

  BOOST_CHECK(c1.size() == 2);
  BOOST_CHECK(c1.count("test1") == 1);
  BOOST_CHECK(c1 == cc1);

  c1.erase(c1.begin(), c1.end());

  BOOST_CHECK(c1 != cc1);
  BOOST_CHECK(c1.empty() == true);
}

BOOST_FIXTURE_TEST_CASE(testSwap, F)
{
  Coll c1;
  c1.str(s1);
  const Coll cc1 = c1;

  Coll c2;
  c2.str(s1);
  const Coll cc2 = c2;

  BOOST_CHECK(c1 == cc1);
  BOOST_CHECK(c2 == cc2);

  c1.swap(c2);

  BOOST_CHECK(c1 == cc2);
  BOOST_CHECK(c2 == cc1);

  c2.swap(c1);

  BOOST_CHECK(c1 == cc1);
  BOOST_CHECK(c2 == cc2);

  c1.clear();
  c2.clear();

  BOOST_CHECK(c1 == c2);
  BOOST_CHECK(c1.str() == "");
  BOOST_CHECK(c2.str() == "");
}

BOOST_FIXTURE_TEST_CASE(testInEquality, F)
{
  Coll c1;
  c1.str(s1);

  Coll c2;
  c2.str(s1);

  BOOST_CHECK(c1 == c1);
  BOOST_CHECK(c2 == c2);
  BOOST_CHECK(c1 == c2);
  BOOST_CHECK(c2 == c1);

  c1.push_back("BLOCK test3");

  BOOST_CHECK(c1 == c1);
  BOOST_CHECK(c2 == c2);
  BOOST_CHECK(c1 != c2);
  BOOST_CHECK(c2 != c1);

  c1.clear();
  c2.clear();

  BOOST_CHECK(c1 == c1);
  BOOST_CHECK(c2 == c2);
  BOOST_CHECK(c1 == c2);
  BOOST_CHECK(c2 == c1);

  c1.push_back("BLOCK test1");
  c2.push_back("BLOCK test2");

  BOOST_CHECK(c1 != c2);
  BOOST_CHECK(c2 != c1);

  c2.clear();
  c2.push_back("BLOCK test1");

  BOOST_CHECK(c1 == c2);
  BOOST_CHECK(c2 == c1);
}

BOOST_AUTO_TEST_SUITE_END()

// vim: sw=2 tw=78
