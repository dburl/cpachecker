/*
 * CPAchecker is a tool for configurable software verification.
 *  This file is part of CPAchecker.
 *
 *  Copyright (C) 2007-2016  Dirk Beyer
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
package org.sosy_lab.cpachecker.core.algorithm.tiger;

import com.google.common.base.Optional;
import com.google.common.base.Preconditions;
import com.google.common.base.Predicates;
import com.google.common.collect.ImmutableMap;
import com.google.common.collect.ImmutableSet;
import com.google.common.collect.ImmutableSet.Builder;
import com.google.common.collect.Maps;
import com.google.common.collect.Sets;

import org.sosy_lab.common.configuration.InvalidConfigurationException;
import org.sosy_lab.common.io.MoreFiles;
import org.sosy_lab.common.log.LogManager;
import org.sosy_lab.cpachecker.cfa.CFA;
import org.sosy_lab.cpachecker.cfa.WeavingLocation;
import org.sosy_lab.cpachecker.cfa.ast.AExpressionStatement;
import org.sosy_lab.cpachecker.cfa.ast.c.CBinaryExpression;
import org.sosy_lab.cpachecker.cfa.model.CFAEdge;
import org.sosy_lab.cpachecker.core.CPAcheckerResult.Result;
import org.sosy_lab.cpachecker.core.algorithm.tgar.interfaces.TestificationOperator;
import org.sosy_lab.cpachecker.core.algorithm.tiger.fql.ast.Edges;
import org.sosy_lab.cpachecker.core.algorithm.tiger.fql.ast.FQLSpecification;
import org.sosy_lab.cpachecker.core.algorithm.tiger.fql.ecp.SingletonECPEdgeSet;
import org.sosy_lab.cpachecker.core.algorithm.tiger.fql.ecp.translators.GuardedEdgeLabel;
import org.sosy_lab.cpachecker.core.algorithm.tiger.fql.ecp.translators.InverseGuardedEdgeLabel;
import org.sosy_lab.cpachecker.core.algorithm.tiger.fql.translators.ecp.CoverageSpecificationTranslator;
import org.sosy_lab.cpachecker.core.algorithm.tiger.goals.Goal;
import org.sosy_lab.cpachecker.core.algorithm.tiger.goals.clustering.InfeasibilityPropagation;
import org.sosy_lab.cpachecker.core.algorithm.tiger.util.TestCase;
import org.sosy_lab.cpachecker.core.algorithm.tiger.util.TestGoalUtils;
import org.sosy_lab.cpachecker.core.algorithm.tiger.util.TestStep;
import org.sosy_lab.cpachecker.core.algorithm.tiger.util.TestStep.AssignmentType;
import org.sosy_lab.cpachecker.core.algorithm.tiger.util.TestStep.VariableAssignment;
import org.sosy_lab.cpachecker.core.algorithm.tiger.util.TestSuite;
import org.sosy_lab.cpachecker.core.algorithm.tiger.util.ThreeValuedAnswer;
import org.sosy_lab.cpachecker.core.algorithm.tiger.util.Wrapper;
import org.sosy_lab.cpachecker.core.counterexample.CFAEdgeWithAssumptions;
import org.sosy_lab.cpachecker.core.counterexample.CFAPathWithAssumptions;
import org.sosy_lab.cpachecker.core.counterexample.CounterexampleInfo;
import org.sosy_lab.cpachecker.core.interfaces.Property;
import org.sosy_lab.cpachecker.core.interfaces.Statistics;
import org.sosy_lab.cpachecker.core.reachedset.ReachedSet;
import org.sosy_lab.cpachecker.cpa.arg.ARGState;
import org.sosy_lab.cpachecker.cpa.arg.ARGToDotWriter;
import org.sosy_lab.cpachecker.cpa.automaton.AutomatonPrecision;
import org.sosy_lab.cpachecker.cpa.automaton.SafetyProperty;
import org.sosy_lab.cpachecker.exceptions.CPAException;
import org.sosy_lab.cpachecker.util.AbstractStates;
import org.sosy_lab.cpachecker.util.Pair;
import org.sosy_lab.cpachecker.util.automaton.NondeterministicFiniteAutomaton;
import org.sosy_lab.cpachecker.util.automaton.NondeterministicFiniteAutomaton.State;
import org.sosy_lab.cpachecker.util.presence.ARGPathWithPresenceConditions;
import org.sosy_lab.cpachecker.util.presence.ARGPathWithPresenceConditions.ForwardPathIteratorWithPresenceConditions;
import org.sosy_lab.cpachecker.util.presence.PathReplayEngine;
import org.sosy_lab.cpachecker.util.presence.PresenceConditions;
import org.sosy_lab.cpachecker.util.presence.interfaces.PresenceCondition;
import org.sosy_lab.cpachecker.util.presence.interfaces.PresenceConditionManager;
import org.sosy_lab.cpachecker.util.statistics.AbstractStatistics;
import org.sosy_lab.cpachecker.util.statistics.StatCpuTime;
import org.sosy_lab.cpachecker.util.statistics.StatCpuTime.StatCpuTimer;
import org.sosy_lab.cpachecker.util.statistics.StatInt;
import org.sosy_lab.cpachecker.util.statistics.StatKind;
import org.sosy_lab.solver.SolverException;
import org.sosy_lab.solver.api.BooleanFormula;

import java.io.IOException;
import java.io.PrintStream;
import java.io.Writer;
import java.math.BigInteger;
import java.nio.charset.Charset;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;
import java.util.logging.Level;
import java.util.regex.Pattern;

import javax.annotation.Nullable;

public class TestGeneration implements Statistics {

  private final GuardedEdgeLabel alphaLabel;
  private final InverseGuardedEdgeLabel inverseAlphaLabel;
  private final GuardedEdgeLabel omegaLabel;
  private final TigerConfiguration cfg;
  private final LogManager logger;

  private CoverageSpecificationTranslator mCoverageSpecificationTranslator;
  private FQLSpecification fqlSpecification;

  private Set<String> inputVariables;
  private Set<String> outputVariables;

  private int statistics_numberOfTestGoals;
  private int statistics_numberOfProcessedTestGoals = 0;

  private final TestSuite testsuite;

  private Map<Goal, List<List<BooleanFormula>>> targetStateFormulas;

  private Set<Goal> remainingGoals = Sets.newLinkedHashSet();

  private final TestGenerationStatistics stats;
  private ImmutableSet<Goal> activeGoalSet;

  private class TestGenerationStatistics extends AbstractStatistics {

    final StatCpuTime acceptsTime = new StatCpuTime();
    final StatCpuTime updateTestsuiteByCoverageOfTime = new StatCpuTime();
    final StatCpuTime createTestcaseTime = new StatCpuTime();
    final StatCpuTime addTestToSuiteTime = new StatCpuTime();
    final StatCpuTime testGenerationTime = new StatCpuTime();
    final StatInt numOfProcessedGoals = new StatInt(StatKind.SUM, "");

    @Override
    public void printStatistics(PrintStream pOut, Result pResult, ReachedSet pReached) {
      super.printStatistics(pOut, pResult, pReached);
      pOut.append("Time for test generation " + testGenerationTime + "\n");
      pOut.append("  Number of processed test goals " + numOfProcessedGoals.getValueSum() + "\n");
      pOut.append("    Time for adding a test to the suite " + addTestToSuiteTime + "\n");
      pOut.append("      Time for creating a test case " + createTestcaseTime + "\n");
      pOut.append("      Time for updating the test coverage " + updateTestsuiteByCoverageOfTime + "\n");
      pOut.append("        Time for checking acceptance " + acceptsTime + "\n");
    }

  }

  public TestGeneration(TigerConfiguration pTigerConfig, CFA pCFA, LogManager pLogger)
      throws InvalidConfigurationException {

    stats = new TestGenerationStatistics();

    cfg = Preconditions.checkNotNull(pTigerConfig);
    logger = Preconditions.checkNotNull(pLogger);

    inputVariables = new TreeSet<>();
    for (String variable : cfg.inputInterface.split(",")) {
      inputVariables.add(variable.trim());
    }

    outputVariables = new TreeSet<>();
    for (String variable : cfg.outputInterface.split(",")) {
      outputVariables.add(variable.trim());
    }

    assert TigerAlgorithm.originalMainFunction != null;
    mCoverageSpecificationTranslator = new CoverageSpecificationTranslator(pCFA.getFunctionHead(TigerAlgorithm.originalMainFunction));

    Wrapper wrapper = new Wrapper(pCFA, TigerAlgorithm.originalMainFunction);

    alphaLabel = new GuardedEdgeLabel(new SingletonECPEdgeSet(wrapper.getAlphaEdge()));
    inverseAlphaLabel = new InverseGuardedEdgeLabel(alphaLabel);
    omegaLabel = new GuardedEdgeLabel(new SingletonECPEdgeSet(wrapper.getOmegaEdge()));

    fqlSpecification = TestGoalUtils.parseFQLQuery(cfg. fqlQuery);
    logger.logf(Level.INFO, "FQL query: %s", fqlSpecification.toString());

    targetStateFormulas = Maps.newHashMap();

    Set<Goal> goalsToCover = TestGoalUtils.extractTestGoalPatterns(fqlSpecification,
        mCoverageSpecificationTranslator, cfg.optimizeGoalAutomata,
        cfg.useOmegaLabel, cfg.useTigerAlgorithm_with_pc, alphaLabel, inverseAlphaLabel, omegaLabel);
    testsuite = new TestSuite(goalsToCover, cfg.printLabels, cfg.useTigerAlgorithm_with_pc);

    statistics_numberOfTestGoals = goalsToCover.size();
    logger.logf(Level.INFO, "Number of test goals: %d", statistics_numberOfTestGoals);

    remainingGoals.addAll(testsuite.getGoals());
  }

  private Set<Goal> goalsUnderAnalysis() {
    return getActiveGoalSet();
  }

  private final TestificationOperator testifier = new TestificationOperator() {
    @Override
    public void feasibleCounterexample(CounterexampleInfo pCounterexample, Set<SafetyProperty> pForProperties)
        throws InterruptedException {

      for (CounterexampleInfo cexi: pCounterexample.getAll()) {
        if (cfg.allCoveredGoalsPerTestCase) {
          addTestToSuite(testsuite.getGoals(), cexi);
        } else if (cfg.checkCoverage) {
          addTestToSuite(Sets.union(getTestSuite().getUncoveredTestGoals(), goalsUnderAnalysis()), cexi);
        } else {
          addTestToSuite(goalsUnderAnalysis(), cexi);
        }
      }

      // Exclude covered goals from further exploration
      Map<SafetyProperty, Optional<PresenceCondition>> toBlacklist = Maps.newHashMap();
      for (Goal goal : goalsUnderAnalysis()) {
        if (testsuite.isGoalCoveredOrInfeasible(goal)) {
          toBlacklist.put(goal, Optional.of(pcm().makeTrue()));
        } else if (cfg.useTigerAlgorithm_with_pc) {
          PresenceCondition remainingPc = testsuite.getRemainingPresenceCondition(goal);
          PresenceCondition coveredFor = pcm().makeNegation(remainingPc);
          toBlacklist.put(goal, Optional.of(coveredFor));
        }
      }

      AutomatonPrecision.updateGlobalPrecision(AutomatonPrecision.getGlobalPrecision()
          .cloneAndAddBlacklisted(toBlacklist));

    }
  };

  public TestificationOperator getTestifier() {
    return testifier;
  }

  @Override
  public String getName() {
    return "Test Generator";
  }

  void dumpTestSuite() {
    if (cfg.testsuiteFile != null) {
      try (Writer writer = MoreFiles.openOutputFile(cfg.testsuiteFile, Charset.defaultCharset())) {
        writer.write(testsuite.toString());
      } catch (IOException e) {
        throw new RuntimeException(e);
      }
    }
  }

  private Pair<Boolean, LinkedList<Edges>> initializeInfisabilityPropagation() {
    Pair<Boolean, LinkedList<Edges>> propagation;

    if (cfg.useInfeasibilityPropagation) {
      propagation = InfeasibilityPropagation.canApplyInfeasibilityPropagation(fqlSpecification);
    } else {
      propagation = Pair.of(Boolean.FALSE, null);
    }

    return propagation;
  }

  public TestSuite getTestSuite() {
    return testsuite;
  }

  public Map<Integer, Pair<Goal, PresenceCondition>> getTimedOutGoals() {
    return testsuite.getTimedOutGoals();
  }

  public void signalGenerationStart() {
  }

  private class AcceptStatus {

    private Goal goal;
    private NondeterministicFiniteAutomaton<GuardedEdgeLabel> automaton;
    private Set<NondeterministicFiniteAutomaton.State> currentStates;
    boolean hasPredicates;
    private ThreeValuedAnswer answer;

    public AcceptStatus(Goal pGoal) {
      goal = pGoal;
      automaton = pGoal.getAutomaton();
      currentStates = Sets.newLinkedHashSet();
      hasPredicates = false;

      currentStates.add(automaton.getInitialState());
    }

    @Override
    public String toString() {
      return goal.getName() + ": " + answer;
    }

  }

  @Nullable
  private Pair<ARGState, PresenceCondition> findStateAfterCriticalEdge(Goal pCriticalForGoal, ARGPathWithPresenceConditions pPath) {
    ForwardPathIteratorWithPresenceConditions it = pPath.iteratorWithPresenceConditions();

    final CFAEdge criticalEdge = pCriticalForGoal.getCriticalEdge();

    while (it.hasNext()) {
      if (it.getOutgoingEdge().equals(criticalEdge)) {
        ARGState afterCritical = it.getNextAbstractState();
        PresenceCondition afterCriticalPc = it.getPresenceCondition();
        while (it.hasNext() && AbstractStates
            .extractLocation(it.getNextAbstractState()) instanceof WeavingLocation) {
          it.advance();
          afterCritical = it.getNextAbstractState();
          afterCriticalPc = it.getPresenceCondition();
          Preconditions.checkState(afterCritical != null);
        }
        return Pair.of(afterCritical, afterCriticalPc);
      }
      it.advance();
    }

    return null;
  }

  private Set<Goal> updateTestsuiteByCoverageOf(TestCase pTestcase,
      ARGPathWithPresenceConditions pArgPath, Set<Goal> pCheckCoverageOf)
    throws InterruptedException {

    try (StatCpuTimer t = stats.updateTestsuiteByCoverageOfTime.start()) {
      Set<Goal> checkCoverageOf = new HashSet<>();
      checkCoverageOf.addAll(pCheckCoverageOf);

      Set<Goal> coveredGoals = Sets.newLinkedHashSet();
      Set<Goal> goalsCoveredByLastState = Sets.newLinkedHashSet();

      ARGState lastState = pTestcase.getArgPath().getLastState();

      for (Property p : lastState.getViolatedProperties()) {
        Preconditions.checkState(p instanceof Goal);
        goalsCoveredByLastState.add((Goal) p);
      }

      checkCoverageOf.removeAll(goalsCoveredByLastState);

      if (!cfg.allCoveredGoalsPerTestCase) {
        for (Goal goal : pCheckCoverageOf) {
          if (testsuite.isGoalCovered(goal)) {
            checkCoverageOf.remove(goal);
          }
        }
      }

      Map<NondeterministicFiniteAutomaton<GuardedEdgeLabel>, AcceptStatus> acceptStati =
          accepts(checkCoverageOf, pTestcase.getErrorPath());

      for (Goal goal : goalsCoveredByLastState) {
        AcceptStatus acceptStatus = new AcceptStatus(goal);
        acceptStatus.answer = ThreeValuedAnswer.ACCEPT;
        acceptStati.put(goal.getAutomaton(), acceptStatus);
      }

      for (NondeterministicFiniteAutomaton<GuardedEdgeLabel> automaton : acceptStati.keySet()) {
        AcceptStatus acceptStatus = acceptStati.get(automaton);
        Goal goal = acceptStatus.goal;

        if (acceptStatus.answer.equals(ThreeValuedAnswer.UNKNOWN)) {
          logger.logf(Level.WARNING,
              "Coverage check for goal %d could not be performed in a precise way!",
              goal.getIndex());
          continue;
        } else if (acceptStatus.answer.equals(ThreeValuedAnswer.REJECT)) {
          continue;
        }

        // test goal is already covered by an existing test case
        if (cfg.useTigerAlgorithm_with_pc) {

          Pair<ARGState, PresenceCondition> critical = findStateAfterCriticalEdge(goal, pArgPath);
          if (critical == null) {
            throw new RuntimeException(String.format(
                "Each ARG path of a counterexample must be along a critical edge! Goal %d has none for edge '%s'",
                goal.getIndex(), goal.getCriticalEdge().toString()));
          }

          Preconditions.checkState(critical.getFirst() != null,
              "Each ARG path of a counterexample must be along a critical edge!");

          PresenceCondition statePresenceCondition = critical.getSecond();

          Preconditions.checkState(statePresenceCondition != null,
              "Each critical state must be annotated with a presence condition!");

          if (cfg.allCoveredGoalsPerTestCase
              || pcm().checkConjunction(testsuite.getRemainingPresenceCondition(goal),
              statePresenceCondition)) {

            // configurations in testGoalPCtoCover and testcase.pc have a non-empty intersection
            testsuite.addTestCase(pTestcase, goal, statePresenceCondition);

            logger.logf(Level.FINE,
                "Covered some PCs for Goal %d (%s) for a PC by test case %d!",
                goal.getIndex(), testsuite.getTestGoalLabel(goal), pTestcase.getId());

            if (pcm().checkEqualsFalse(testsuite.getRemainingPresenceCondition(goal))) {
              coveredGoals.add(goal);
            }
          }

        } else {
          testsuite.addTestCase(pTestcase, goal, null);
          logger.logf(Level.FINE, "Covered Goal %d (%s) by test case %d!",
              goal.getIndex(),
              testsuite.getTestGoalLabel(goal),
              pTestcase.getId());
          coveredGoals.add(goal);
        }
      }

      return coveredGoals;
    }
  }

  private PresenceConditionManager pcm() {
    return PresenceConditions.manager();
  }

  private Map<NondeterministicFiniteAutomaton<GuardedEdgeLabel>, AcceptStatus> accepts(
      Collection<Goal> pGoals, List<CFAEdge> pErrorPath) {

    try (StatCpuTimer t = stats.acceptsTime.start()) {
      Map<NondeterministicFiniteAutomaton<GuardedEdgeLabel>, AcceptStatus> map = new HashMap<>();
      Set<NondeterministicFiniteAutomaton.State> lNextStates = Sets.newLinkedHashSet();

      Set<NondeterministicFiniteAutomaton<GuardedEdgeLabel>> automataWithResult = new HashSet<>();

      for (Goal goal : pGoals) {
        AcceptStatus acceptStatus = new AcceptStatus(goal);
        map.put(goal.getAutomaton(), acceptStatus);
        if (acceptStatus.automaton.getFinalStates()
            .contains(acceptStatus.automaton.getInitialState())) {
          acceptStatus.answer = ThreeValuedAnswer.ACCEPT;
          automataWithResult.add(acceptStatus.automaton);
        }
      }

      for (CFAEdge lCFAEdge : pErrorPath) {
        List<NondeterministicFiniteAutomaton<GuardedEdgeLabel>> automata = testsuite.getTGAForEdge(lCFAEdge);
        if (automata == null) {
          continue;
        }

        for (NondeterministicFiniteAutomaton<GuardedEdgeLabel> automaton : automata) {
          if (automataWithResult.contains(automaton)) {
            continue;
          }

          AcceptStatus acceptStatus = map.get(automaton);
          if (acceptStatus == null) {
            continue;
          }
          for (NondeterministicFiniteAutomaton.State lCurrentState : acceptStatus.currentStates) {
            for (NondeterministicFiniteAutomaton<GuardedEdgeLabel>.Edge lOutgoingEdge : automaton
                .getOutgoingEdges(lCurrentState)) {
              GuardedEdgeLabel lLabel = lOutgoingEdge.getLabel();

              if (lLabel.hasGuards()) {
                acceptStatus.hasPredicates = true;
              } else {
                if (lLabel.contains(lCFAEdge)) {
                  lNextStates.add(lOutgoingEdge.getTarget());
                  lNextStates.addAll(
                      getSuccsessorsOfEmptyTransitions(automaton, lOutgoingEdge.getTarget()));

                  for (State nextState : lNextStates) {
                    // Automaton accepts as soon as it sees a final state (implicit self-loop)
                    if (automaton.getFinalStates().contains(nextState)) {
                      acceptStatus.answer = ThreeValuedAnswer.ACCEPT;
                      automataWithResult.add(automaton);
                    }
                  }
                }
              }
            }
          }

          acceptStatus.currentStates.addAll(lNextStates);
          lNextStates.clear();
        }
      }

      for (NondeterministicFiniteAutomaton<GuardedEdgeLabel> autom : map.keySet()) {
        if (automataWithResult.contains(autom)) {
          continue;
        }

        AcceptStatus accepts = map.get(autom);
        if (accepts.hasPredicates) {
          accepts.answer = ThreeValuedAnswer.UNKNOWN;
        } else {
          accepts.answer = ThreeValuedAnswer.REJECT;
        }
      }

      return map;
    }
  }

  private static Collection<? extends State> getSuccsessorsOfEmptyTransitions(
      NondeterministicFiniteAutomaton<GuardedEdgeLabel> pAutomaton, State pState) {
    Set<State> states = new HashSet<>();
    for (NondeterministicFiniteAutomaton<GuardedEdgeLabel>.Edge edge : pAutomaton
        .getOutgoingEdges(pState)) {
      GuardedEdgeLabel label = edge.getLabel();
      if (Pattern.matches("E\\d+ \\[\\]", label.toString())) {
        states.add(edge.getTarget());
      }
    }
    return states;
  }

  /**
   * Context:
   *  The analysis has identified a feasible counterexample, i.e., a test case.
   *
   * Add the test case to the test suite. This includes:
   *  * Register the test case for the goals that it reached on its last abstract state.
   *  * Add the test case for the goals that it would also cover;
   *    this gets checked by running all (uncovered) goal automata on the ARG path of the test case.
   *
   * @param pRemainingGoals
   * @param pCex
   *
   * @throws InterruptedException
   * @throws SolverException
   */
  private Set<Goal> addTestToSuite(Set<Goal> pRemainingGoals, CounterexampleInfo pCex)
      throws InterruptedException {

    try (StatCpuTimer t = stats.addTestToSuiteTime.start()) {
      Preconditions.checkNotNull(pRemainingGoals);
      Preconditions.checkNotNull(pCex);

      ARGPathWithPresenceConditions argPath = computePathWithPresenceConditions(pRemainingGoals, pCex);

      // TODO check whether a last state might remain from an earlier run and a reuse of the ARG

      PresenceCondition testCasePresenceCondition = argPath.getLastPresenceCondition();
      TestCase testcase = createTestcase(pCex, testCasePresenceCondition);

      return updateTestsuiteByCoverageOf(testcase, argPath, pRemainingGoals);
    }
  }


  private ARGPathWithPresenceConditions computePathWithPresenceConditions(Set<Goal> goals, CounterexampleInfo pCex)
      throws InterruptedException {

    try {
      PathReplayEngine replayer = new PathReplayEngine(logger);
      return replayer.replayPath(pCex.getTargetPath());
    } catch (CPAException e) {
      throw new RuntimeException("CPA for handling features could not be created.");
    }
  }


  private TestCase createTestcase(final CounterexampleInfo pCex, final PresenceCondition pPresenceCondition) {

    try (StatCpuTimer t = stats.createTestcaseTime.start()) {
      CFAPathWithAssumptions model = pCex.getCFAPathWithAssignments();
      final List<TestStep> testSteps = calculateTestSteps(model);

      TestCase testcase = new TestCase(testSteps, pCex.getTargetPath(), pCex.getTargetPath().getInnerEdges(), pPresenceCondition);

      Set<Property> props = pCex.getTargetPath().getLastState().getViolatedProperties();

      if (cfg.useTigerAlgorithm_with_pc) {
        logger.logf(Level.FINE, "Generated new test case %d for %s with a PC in the last state.",
            testcase.getId(), props);
      } else {
        logger.logf(Level.FINE, "Generated new test case %d for %s.", testcase.getId(), props);
      }

      return testcase;
    }
  }

  private List<TestStep> calculateTestSteps(CFAPathWithAssumptions path) {
    List<TestStep> testSteps = new ArrayList<>();

    boolean lastValueWasOuput = true;
    TestStep curStep = null;

    for (CFAEdgeWithAssumptions edge : path) {
      Collection<AExpressionStatement> expStmts = edge.getExpStmts();
      for (AExpressionStatement expStmt : expStmts) {
        if (expStmt.getExpression() instanceof CBinaryExpression) {
          CBinaryExpression exp = (CBinaryExpression) expStmt.getExpression();

          if (inputVariables.contains(exp.getOperand1().toString())) {
            if (lastValueWasOuput) {
              if (curStep != null) {
                testSteps.add(curStep);
              }
              curStep = new TestStep();
            }

            String variableName = exp.getOperand1().toString();
            BigInteger value = new BigInteger(exp.getOperand2().toString());
            VariableAssignment input =
                new VariableAssignment(variableName, value, AssignmentType.INPUT);
            curStep.addAssignment(input);

            lastValueWasOuput = false;
          } else if (outputVariables.contains(exp.getOperand1().toString())) {
            if (curStep == null) {
              curStep = new TestStep();
            }

            String variableName = exp.getOperand1().toString();
            BigInteger value = new BigInteger(exp.getOperand2().toString());
            VariableAssignment input =
                new VariableAssignment(variableName, value, AssignmentType.OUTPUT);
            curStep.addAssignment(input);

            lastValueWasOuput = true;
          }
        }
      }
    }

    if (curStep != null) {
      testSteps.add(curStep);
    }

    return testSteps;
  }


  void handleInfeasibleTestGoal(Goal pGoal) {
    if (cfg.useTigerAlgorithm_with_pc) {
      testsuite.addInfeasibleGoal(pGoal, testsuite.getRemainingPresenceCondition(pGoal));
      logger.logf(Level.FINE, "Goal %d is infeasible for remaining PC!", pGoal.getIndex());
    } else {
      logger.logf(Level.FINE, "Goal %d is infeasible!", pGoal.getIndex());
      testsuite.addInfeasibleGoal(pGoal, null);
    }
  }

  private static TestCase getLastTestCase(List<TestCase> pTests) {
    TestCase lastTestCase = null;
    for (TestCase testCase : pTests) {
      if (lastTestCase == null || testCase.getGenerationTime() < lastTestCase.getGenerationTime()) {
        lastTestCase = testCase;
      }
    }
    return lastTestCase;
  }

  @Override
  public void printStatistics(PrintStream pOut, Result pResult, ReachedSet pReached) {

    stats.printStatistics(pOut, pResult, pReached);

    // fix test suite and set timedout goals as timedout
    // TODO: move this fix to a more adequate place
    // TODO: fix for variability aware
    Set<Goal> goals = testsuite.getGoals();
    Set<Goal> timedout = new HashSet<>();
    for (Goal goal : goals) {
      try {
        if (!testsuite.isGoalCoveredOrInfeasible(goal)) {
          timedout.add(goal);
        }
      } catch (InterruptedException e) {}
    }
    testsuite.addTimedOutGoals(timedout);

    pOut.println(
        "Number of test cases:                              " + testsuite.getNumberOfTestCases());
    pOut.println(
        "Number of test goals:                              " + statistics_numberOfTestGoals);
    pOut.println("Number of processed test goals:                    "
        + statistics_numberOfProcessedTestGoals);

    if (cfg.useTigerAlgorithm_with_pc) {
      pOut.println("Number of feasible test goals:                     "
          + testsuite.getNumberOfFeasibleTestGoals());
      pOut.println("Number of partially feasible test goals:           "
          + testsuite.getNumberOfPartiallyFeasibleTestGoals());
      pOut.println("Number of infeasible test goals:                   "
          + testsuite.getNumberOfInfeasibleTestGoals());
      pOut.println("Number of partially infeasible test goals:         "
          + testsuite.getNumberOfPartiallyInfeasibleTestGoals());
      pOut.println("Number of timedout test goals:                     "
          + testsuite.getNumberOfTimedoutTestGoals());
      pOut.println("Number of partially timedout test goals:           "
          + testsuite.getNumberOfPartiallyTimedOutTestGoals());

      if (testsuite.getNumberOfTimedoutTestGoals() > 0
          || testsuite.getNumberOfPartiallyTimedOutTestGoals() > 0) {
        pOut.println("Timeout occured during processing of a test goal!");
      }
    } else {
      pOut.println("Number of feasible test goals:                     "
          + testsuite.getNumberOfFeasibleTestGoals());
      pOut.println("Number of infeasible test goals:                   "
          + testsuite.getNumberOfInfeasibleTestGoals());
      pOut.println("Number of timedout test goals:                     "
          + testsuite.getNumberOfTimedoutTestGoals());

      if (testsuite.getNumberOfTimedoutTestGoals() > 0) {
        pOut.println("Timeout occured during processing of a test goal!");
      }
    }

    dumpTestSuite();

    if (cfg.printPathFormulasPerGoal) {
      dumpPathFormulas();
    }

    // write test case generation times to file system
    if (cfg.testcaseGenerationTimesFile != null) {
      try (Writer writer =
               MoreFiles.openOutputFile(cfg.testcaseGenerationTimesFile, Charset.defaultCharset())) {

        List<TestCase> testcases = new ArrayList<>(testsuite.getTestCases());
        Collections.sort(testcases, new Comparator<TestCase>() {

          @Override
          public int compare(TestCase pTestCase1, TestCase pTestCase2) {
            if (pTestCase1.getGenerationTime() > pTestCase2.getGenerationTime()) {
              return 1;
            } else if (pTestCase1.getGenerationTime() < pTestCase2
                .getGenerationTime()) { return -1; }
            return 0;
          }
        });

        if (cfg.useTigerAlgorithm_with_pc) {
          Set<Goal> feasible = Sets.newLinkedHashSet();
          feasible.addAll(testsuite.getFeasibleGoals());
          feasible.addAll(testsuite.getPartiallyFeasibleGoals());
          feasible.removeAll(testsuite.getPartiallyTimedOutGoals());
          for (Goal goal : feasible) {
            List<TestCase> tests = testsuite.getCoveringTestCases(goal);
            TestCase lastTestCase = getLastTestCase(tests);
            lastTestCase.incrementNumberOfNewlyCoveredGoals();
          }
          Set<Goal> partially = Sets.newLinkedHashSet();
          partially.addAll(testsuite.getFeasibleGoals());
          partially.addAll(testsuite.getPartiallyFeasibleGoals());
          partially.removeAll(testsuite.getPartiallyTimedOutGoals());
          for (Goal goal : partially) {
            List<TestCase> tests = testsuite.getCoveringTestCases(goal);
            TestCase lastTestCase = getLastTestCase(tests);
            lastTestCase.incrementNumberOfNewlyPartiallyCoveredGoals();
          }

          writer.write(
              "Test Case;Generation Time;Covered Goals After Generation;Completely Covered Goals After Generation;Partially Covered Goals After Generation\n");
          int completelyCoveredGoals = 0;
          int partiallyCoveredGoals = 0;
          for (TestCase testCase : testcases) {
            int newCoveredGoals = testCase.getNumberOfNewlyCoveredGoals();
            int newPartiallyCoveredGoals = testCase.getNumberOfNewlyPartiallyCoveredGoals();
            completelyCoveredGoals += newCoveredGoals;
            partiallyCoveredGoals += newPartiallyCoveredGoals;

            writer.write(testCase.getId() + ";" + testCase.getGenerationTime() + ";"
                + (completelyCoveredGoals + partiallyCoveredGoals) + ";" + completelyCoveredGoals
                + ";"
                + partiallyCoveredGoals + "\n");
          }
        } else {
          Set<Goal> coveredGoals = Sets.newLinkedHashSet();
          writer.write("Test Case;Generation Time;Covered Goals After Generation\n");
          for (TestCase testCase : testcases) {
            coveredGoals.addAll(testsuite.getTestGoalsCoveredByTestCase(testCase));
            writer.write(testCase.getId() + ";" + testCase.getGenerationTime() + ";"
                + coveredGoals.size() + "\n");
          }
        }
      } catch (IOException e) {
        throw new RuntimeException(e);
      }
    }
  }

  private void dumpPathFormulas() {
    if (cfg.pathFormulaFile != null) {
      StringBuffer buffer = new StringBuffer();
      for (Goal goal : targetStateFormulas.keySet()) {
        buffer.append("GOAL " + goal + "\n");
        for (List<BooleanFormula> formulas : targetStateFormulas.get(goal)) {
          buffer.append("FORMULA\n");
          for (BooleanFormula formula : formulas) {
            buffer.append(formula + "\n");
          }
        }
      }

      try (Writer writer = MoreFiles.openOutputFile(cfg.pathFormulaFile, Charset.defaultCharset())) {
        writer.write(buffer.toString());
      } catch (IOException e) {
        throw new RuntimeException(e);
      }
    }
  }

  ImmutableSet<Goal> nextTestGoalSet(Set<Goal> pGoalsToCover) {
    final int testGoalSetSize = (cfg.numberOfTestGoalsPerRun <= 0)
            ? pGoalsToCover.size()
            : (pGoalsToCover.size() > cfg.numberOfTestGoalsPerRun)
              ? cfg.numberOfTestGoalsPerRun
               : pGoalsToCover.size();

    Builder<Goal> builder = ImmutableSet.builder();

    Iterator<Goal> it = pGoalsToCover.iterator();
    for (int i = 0; i < testGoalSetSize; i++) {
      if (it.hasNext()) {
        builder.add(it.next());
      }
    }

    activeGoalSet = builder.build();

    return activeGoalSet;
  }

  public void setActiveGoalSet(Set<Goal> pActiveGoalSet) {
    activeGoalSet = ImmutableSet.copyOf(pActiveGoalSet);
  }

  public ImmutableSet<Goal> getActiveGoalSet() {
    return activeGoalSet;
  }

  public void removeGoalsFromRemaining(Set<Goal> pToRemove) {
    remainingGoals.removeAll(pToRemove);
  }

  public ImmutableSet<Goal> getRemainingGoals() {
    return ImmutableSet.copyOf(remainingGoals);
  }
}