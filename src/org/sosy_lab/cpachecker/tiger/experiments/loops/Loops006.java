/*
 *  CPAchecker is a tool for configurable software verification.
 *  This file is part of CPAchecker.
 *
 *  Copyright (C) 2007-2011  Dirk Beyer
 *  All rights reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *
 *  CPAchecker web page:
 *    http://cpachecker.sosy-lab.org
 */
package org.sosy_lab.cpachecker.tiger.experiments.loops;

import org.junit.Assert;
import org.junit.Test;
import org.sosy_lab.cpachecker.tiger.CPAtigerResult;
import org.sosy_lab.cpachecker.tiger.cmdline.Main;
import org.sosy_lab.cpachecker.tiger.experiments.ExperimentalSeries;
import org.sosy_lab.cpachecker.tiger.fql.PredefinedCoverageCriteria;

public class Loops006 extends ExperimentalSeries {

  @Test
  public void systemc_001() throws Exception {
    String lCFile = "loops6.c";

    String[] lArguments = Main.getParameters(PredefinedCoverageCriteria.STATEMENT_COVERAGE,
                                        "test/programs/fql/loops/" + lCFile,
                                        "main",
                                        true);

    CPAtigerResult lResult = execute(lArguments);

    // Summary successor has 0 entering CFA edges ... we can not determine an omega edge, but, all test goals are infeasible anyways!
    Assert.assertEquals(80, lResult.getNumberOfTestGoals());
    Assert.assertEquals(74, lResult.getNumberOfFeasibleTestGoals());
    Assert.assertEquals(6, lResult.getNumberOfInfeasibleTestGoals());
    Assert.assertEquals(12, lResult.getNumberOfTestCases());
    Assert.assertEquals(0, lResult.getNumberOfImpreciseTestCases());
  }

}
