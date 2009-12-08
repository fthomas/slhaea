// SLHAea - SUSY Les Houches Accord made easy
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

#include <cppunit/ui/text/TestRunner.h>

#include "testslha.h"
#include "testslhablock.h"
#include "testslhaline.h"

int main()
{
  CppUnit::TextUi::TestRunner runner;

  runner.addTest(SLHAea::TestSLHALine::suite());
  runner.addTest(SLHAea::TestSLHABlock::suite());
  runner.addTest(SLHAea::TestSLHA::suite());
  runner.run();

  return 0;
}

// vim: sw=2 tw=78
