/*
 *  CPAchecker is a tool for configurable software verification.
 *  This file is part of CPAchecker. 
 *
 *  Copyright (C) 2007-2008  Dirk Beyer and Erkan Keremoglu.
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
 *    http://www.cs.sfu.ca/~dbeyer/CPAchecker/
 */
package cpa.symbpredabsCPA;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Deque;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.logging.Level;

import symbpredabstraction.PathFormula;
import symbpredabstraction.SSAMap;
import symbpredabstraction.Cache.CartesianAbstractionCacheKey;
import symbpredabstraction.Cache.FeasibilityCacheKey;
import symbpredabstraction.Cache.TimeStampCache;
import symbpredabstraction.interfaces.AbstractFormula;
import symbpredabstraction.interfaces.AbstractFormulaManager;
import symbpredabstraction.interfaces.InterpolatingTheoremProver;
import symbpredabstraction.interfaces.Predicate;
import symbpredabstraction.interfaces.SymbolicFormula;
import symbpredabstraction.interfaces.TheoremProver;
import symbpredabstraction.mathsat.BDDMathsatAbstractFormulaManager;
import symbpredabstraction.mathsat.MathsatAbstractionPrinter;
import symbpredabstraction.mathsat.MathsatSymbolicFormula;
import symbpredabstraction.mathsat.MathsatSymbolicFormulaManager;
import symbpredabstraction.trace.ConcreteTraceFunctionCalls;
import symbpredabstraction.trace.CounterexampleTraceInfo;
import cfa.objectmodel.CFAFunctionDefinitionNode;
import cmdline.CPAMain;

import common.Pair;

import exceptions.CPAException;
import exceptions.RefinementFailedException;


/**
 * Implementation of SummaryAbstractFormulaManager that works with BDDs for
 * abstraction and MathSAT terms for concrete formulas
 *
 * @author Alberto Griggio <alberto.griggio@disi.unitn.it>
 */

class BDDMathsatSymbPredAbstractionAbstractManager<T> extends BDDMathsatAbstractFormulaManager 
implements SymbPredAbstFormulaManager
{

  static class Stats {
    public long abstractionMathsatTime = 0;
    public long abstractionMaxMathsatTime = 0;
    public long abstractionBddTime = 0;
    public long abstractionMaxBddTime = 0;
    public int numCallsAbstraction = 0;
    public int numCallsAbstractionCached = 0;
    public long cexAnalysisTime = 0;
    public long cexAnalysisMaxTime = 0;
    public int numCallsCexAnalysis = 0;
    public long abstractionMathsatSolveTime = 0;
    public long abstractionMaxMathsatSolveTime = 0;
    public long cexAnalysisMathsatTime = 0;
    public long cexAnalysisMaxMathsatTime = 0;
    public int numCoverageChecks = 0;
    public long bddCoverageCheckTime = 0;
    public long bddCoverageCheckMaxTime = 0;
    public long cexAnalysisGetUsefulBlocksTime = 0;
    public long cexAnalysisGetUsefulBlocksMaxTime = 0;
    public long replacing = 0;
  }
  final Stats stats;

  private final TheoremProver thmProver;
  private final InterpolatingTheoremProver<T> itpProver;

  private static final int MAX_CACHE_SIZE = 100000;

  private final boolean cartesianAbstraction;
  private final boolean dumpHardAbstractions;
  private final boolean getUsefulBlocks;
  private final boolean shortestTrace;
  private final boolean splitItpAtoms;
  private final boolean useBitwiseAxioms;
  private final boolean useCache;
  private final boolean useDtc;
  private final boolean useSuffix;
  private final boolean useZigZag;
  private final boolean wellScopedPredicates;
  
  private final Map<Pair<SymbolicFormula, List<SymbolicFormula>>, AbstractFormula> abstractionCache;
  //cache for cartesian abstraction queries. For each predicate, the values
  // are -1: predicate is false, 0: predicate is don't care,
  // 1: predicate is true
  private final TimeStampCache<CartesianAbstractionCacheKey, Byte> cartesianAbstractionCache;
  private final TimeStampCache<FeasibilityCacheKey, Boolean> feasibilityCache;

  public BDDMathsatSymbPredAbstractionAbstractManager(
      AbstractFormulaManager pAmgr,
      MathsatSymbolicFormulaManager pMmgr,
      TheoremProver pThmProver,
      InterpolatingTheoremProver<T> pItpProver) {
    super(pAmgr, pMmgr);
    stats = new Stats();
    thmProver = pThmProver;
    itpProver = pItpProver;

    cartesianAbstraction = CPAMain.cpaConfig.getBooleanValue("cpas.symbpredabs.abstraction.cartesian");
    dumpHardAbstractions = CPAMain.cpaConfig.getBooleanValue("cpas.symbpredabs.mathsat.dumpHardAbstractionQueries");
    getUsefulBlocks      = CPAMain.cpaConfig.getBooleanValue("cpas.symbpredabs.explicit.getUsefulBlocks");
    shortestTrace        = CPAMain.cpaConfig.getBooleanValue("cpas.symbpredabs.shortestCexTrace");
    splitItpAtoms        = CPAMain.cpaConfig.getBooleanValue("cpas.symbpredabs.refinement.splitItpAtoms");
    useBitwiseAxioms     = CPAMain.cpaConfig.getBooleanValue("cpas.symbpredabs.useBitwiseAxioms");
    useCache             = !CPAMain.cpaConfig.getBooleanValue("cpas.symbpredabs.mathsat.useCache");
    useDtc               = CPAMain.cpaConfig.getBooleanValue("cpas.symbpredabs.mathsat.useDtc");
    useSuffix            = CPAMain.cpaConfig.getBooleanValue("cpas.symbpredabs.shortestCexTraceUseSuffix");
    useZigZag            = CPAMain.cpaConfig.getBooleanValue("cpas.symbpredabs.shortestCexTraceZigZag");
    wellScopedPredicates = CPAMain.cpaConfig.getBooleanValue("cpas.symbpredabs.refinement.addWellScopedPredicates");
    
    if (useCache) {
      if (cartesianAbstraction) {
        abstractionCache = null;
        cartesianAbstractionCache = new TimeStampCache<CartesianAbstractionCacheKey, Byte>(MAX_CACHE_SIZE);
        feasibilityCache = new TimeStampCache<FeasibilityCacheKey, Boolean>(MAX_CACHE_SIZE);
      
      } else {
        abstractionCache = new HashMap<Pair<SymbolicFormula, List<SymbolicFormula>>, AbstractFormula>();
        cartesianAbstractionCache = null;
        feasibilityCache = null;
      }
    } else {
      abstractionCache = null;
      cartesianAbstractionCache = null;
      feasibilityCache = null;
    }
  }

  @Override
  public AbstractFormula buildAbstraction(
      AbstractFormula abs, PathFormula pathFormula,
      Collection<Predicate> predicates) {
    stats.numCallsAbstraction++;
    if (cartesianAbstraction) {
      return buildCartesianAbstraction(abs, pathFormula, predicates);
    } else {
      return buildBooleanAbstraction(abs, pathFormula, predicates);
    }
  }
  
  private AbstractFormula buildCartesianAbstraction(
      AbstractFormula abs,
      PathFormula pathFormula,
      Collection<Predicate> predicates) {
    
    long startTime = System.currentTimeMillis();

    final SymbolicFormula absFormula = smgr.instantiate(toConcrete(abs), null);
    final SSAMap absSsa = mmgr.extractSSA((MathsatSymbolicFormula)absFormula);
    
    
    pathFormula = smgr.shift(pathFormula.getSymbolicFormula(), absSsa);
    final SymbolicFormula symbFormula = mmgr.replaceAssignments((MathsatSymbolicFormula)pathFormula.getSymbolicFormula());
    final SSAMap ssa = pathFormula.getSsa();
    
    SymbolicFormula f = smgr.makeAnd(absFormula, symbFormula);
    
//    Pair<SymbolicFormula, SSAMap> pc =
//        buildConcreteFormula(mmgr, e, succ, edge, false);
//    SymbolicFormula f = pc.getFirst();
//    SSAMap ssa = pc.getSecond();
//    SymbolicFormula f = pathFormula.getSymbolicFormula();
//    SSAMap ssa = pathFormula.getSsa();
//
//    f = mmgr.replaceAssignments((MathsatSymbolicFormula)f);
    SymbolicFormula fkey = f;

    byte[] predVals = null;
    final byte NO_VALUE = -2;
    if (useCache) {
        predVals = new byte[predicates.size()];
        int predIndex = -1;
        for (Predicate p : predicates) {
            ++predIndex;
            CartesianAbstractionCacheKey key =
                new CartesianAbstractionCacheKey(f, p);
            if (cartesianAbstractionCache.containsKey(key)) {
                predVals[predIndex] = cartesianAbstractionCache.get(key);
            } else {
                predVals[predIndex] = NO_VALUE;
            }
        }
    }

    boolean skipFeasibilityCheck = false;
    if (useCache) {
        FeasibilityCacheKey key = new FeasibilityCacheKey(fkey);
        if (feasibilityCache.containsKey(key)) {
            skipFeasibilityCheck = true;
            if (!feasibilityCache.get(key)) {
                // abstract post leads to false, we can return immediately
                return amgr.makeFalse();
            }
        }
    }

    if (useBitwiseAxioms) {
        SymbolicFormula bitwiseAxioms = mmgr.getBitwiseAxioms((MathsatSymbolicFormula)f);
        f = mmgr.makeAnd(f, bitwiseAxioms);

        CPAMain.logManager.log(Level.ALL, "DEBUG_3", "ADDED BITWISE AXIOMS:",
                bitwiseAxioms);
    }

    long solveStartTime = System.currentTimeMillis();

    thmProver.init(TheoremProver.CARTESIAN_ABSTRACTION);
    try {
      
      if (!skipFeasibilityCheck) {
        //++stats.abstractionNumMathsatQueries;
        boolean unsat = thmProver.isUnsat(f);
        if (useCache) {
          FeasibilityCacheKey key = new FeasibilityCacheKey(fkey);
          feasibilityCache.put(key, !unsat);
        }
        if (unsat) {
          return amgr.makeFalse();
        }
      } else {
          //++stats.abstractionNumCachedQueries;
      }
  
      thmProver.push(f);
      try {
        long totBddTime = 0;
    
        AbstractFormula absbdd = amgr.makeTrue();
    
        // check whether each of the predicate is implied in the next state...
        Set<String> predvars = new HashSet<String>();
        Set<Pair<String, SymbolicFormula[]>> predlvals =
            new HashSet<Pair<String, SymbolicFormula[]>>();
        Map<SymbolicFormula, SymbolicFormula> predLvalsCache =
            new HashMap<SymbolicFormula, SymbolicFormula>();
    
        int predIndex = -1;
        for (Predicate p : predicates) {
          ++predIndex;
          if (useCache && predVals[predIndex] != NO_VALUE) {
            long startBddTime = System.currentTimeMillis();
            AbstractFormula v = p.getFormula();
            if (predVals[predIndex] == -1) { // pred is false
                v = amgr.makeNot(v);
                absbdd = amgr.makeAnd(absbdd, v);
            } else if (predVals[predIndex] == 1) { // pred is true
                absbdd = amgr.makeAnd(absbdd, v);
            }
            long endBddTime = System.currentTimeMillis();
            totBddTime += (endBddTime - startBddTime);
            //++stats.abstractionNumCachedQueries;
          } else {
            Pair<? extends SymbolicFormula, ? extends SymbolicFormula> pi =
                getPredicateVarAndAtom(p);

            // update the SSA map, by instantiating all the uninstantiated
            // variables that occur in the predicates definitions
            // (at index 1)
            predvars.clear();
            predlvals.clear();
            collectVarNames(pi.getSecond(),
                    predvars, predlvals);
            for (String var : predvars) {
                if (ssa.getIndex(var) < 0) {
                    ssa.setIndex(var, 1);
                }
            }
            for (Pair<String, SymbolicFormula[]> pp : predlvals) {
                SymbolicFormula[] args =
                    getInstantiatedAt(pp.getSecond(), ssa,
                            predLvalsCache);
                if (ssa.getIndex(pp.getFirst(), args) < 0) {
                    ssa.setIndex(pp.getFirst(), args, 1);
                }
            }


            CPAMain.logManager.log(Level.ALL, "DEBUG_1",
                    "CHECKING VALUE OF PREDICATE: ", pi.getFirst());

            // instantiate the definition of the predicate
            SymbolicFormula predTrue = mmgr.instantiate(pi.getSecond(), ssa);
            SymbolicFormula predFalse = smgr.makeNot(predTrue);

            // check whether this predicate has a truth value in the next
            // state
            byte predVal = 0; // pred is neither true nor false

            //++stats.abstractionNumMathsatQueries;
            boolean isTrue = thmProver.isUnsat(predFalse);

            if (isTrue) {
                long startBddTime = System.currentTimeMillis();
                AbstractFormula v = p.getFormula();
                absbdd = amgr.makeAnd(absbdd, v);
                long endBddTime = System.currentTimeMillis();
                totBddTime += (endBddTime - startBddTime);
                
                predVal = 1; 
            } else {
                // check whether it's false...
                //++stats.abstractionNumMathsatQueries;
              boolean isFalse = thmProver.isUnsat(predTrue);

                if (isFalse) {
                    long startBddTime = System.currentTimeMillis();
                    AbstractFormula v = p.getFormula();
                    v = amgr.makeNot(v);
                    absbdd = amgr.makeAnd(absbdd, v);
                    long endBddTime = System.currentTimeMillis();
                    totBddTime += (endBddTime - startBddTime);
                    
                    predVal = -1;
                }
            }

            if (useCache) {
                CartesianAbstractionCacheKey key =
                    new CartesianAbstractionCacheKey(fkey, p);
                cartesianAbstractionCache.put(key, predVal);
            }
          }
        }
        long solveEndTime = System.currentTimeMillis();

        // update statistics
        long endTime = System.currentTimeMillis();
        long solveTime = (solveEndTime - solveStartTime) - totBddTime;
        long msatTime = (endTime - startTime) - totBddTime;
        stats.abstractionMaxMathsatTime =
            Math.max(msatTime, stats.abstractionMaxMathsatTime);
        stats.abstractionMaxBddTime =
            Math.max(totBddTime, stats.abstractionMaxBddTime);
        stats.abstractionMathsatTime += msatTime;
        stats.abstractionBddTime += totBddTime;
        stats.abstractionMathsatSolveTime += solveTime;
        stats.abstractionMaxMathsatSolveTime =
            Math.max(solveTime, stats.abstractionMaxMathsatSolveTime);
        
        return absbdd;

      } finally {
        thmProver.pop();
      }

    } finally {
      thmProver.reset();     
    }
  }

  private AbstractFormula buildBooleanAbstraction(
      AbstractFormula abstractionFormula, PathFormula pathFormula,
      Collection<Predicate> predicates) {
    // A SummaryFormulaManager for MathSAT formulas
    
    CPAMain.logManager.log(Level.ALL, "Old abstraction:", abstractionFormula);
    CPAMain.logManager.log(Level.ALL, "Path formula:", pathFormula);
    CPAMain.logManager.log(Level.ALL, "Predicates:", predicates);
    
    long startTime = System.currentTimeMillis();

    // build the concrete representation of the abstract formula of e
    // this is an abstract formula - specifically it is a bddabstractformula
    // which is basically an integer which represents it
    // create the concrete form of the abstract formula
    // (abstract formula is the bdd representation)
    final SymbolicFormula absFormula = smgr.instantiate(toConcrete(abstractionFormula), null);

    // create an ssamap from concrete formula
    SSAMap absSsa = mmgr.extractSSA((MathsatSymbolicFormula)absFormula);

    // shift pathFormula indices by the offsets in absSsa
    long start = System.currentTimeMillis();

    pathFormula = smgr.shift(pathFormula.getSymbolicFormula(), absSsa);
    SymbolicFormula symbFormula = mmgr.replaceAssignments((MathsatSymbolicFormula)pathFormula.getSymbolicFormula());
    final SSAMap symbSsa = pathFormula.getSsa();

    long end = System.currentTimeMillis();
    stats.replacing += (end - start);
    
    // from now on, abstractionFormula, pathFormula and functionExitFormula should not be used,
    // only absFormula, absSsa, symbFormula, symbSsa
    
    if (useBitwiseAxioms) {
      MathsatSymbolicFormula bitwiseAxioms = mmgr.getBitwiseAxioms(
          (MathsatSymbolicFormula)symbFormula);
      symbFormula = smgr.makeAnd(symbFormula, bitwiseAxioms);

      CPAMain.logManager.log(Level.ALL, "DEBUG_3", "ADDED BITWISE AXIOMS:",
          bitwiseAxioms);
    }


    // first, create the new formula corresponding to
    // (symbFormula & edges from e to succ)
    // TODO - at the moment, we assume that all the edges connecting e and
    // succ have no statement or assertion attached (i.e. they are just
    // return edges or gotos). This might need to change in the future!!
    // (So, for now we don't need to to anything...)


    // build the definition of the predicates, and instantiate them
    PredicateInfo predinfo = buildPredicateInformation(predicates);
    {
      // update the SSA map, by instantiating all the uninstantiated
      // variables that occur in the predicates definitions (at index 1)
      for (String var : predinfo.allVariables) {
        if (symbSsa.getIndex(var) < 0) {
          symbSsa.setIndex(var, 1);
        }
      }
      Map<SymbolicFormula, SymbolicFormula> cache =
        new HashMap<SymbolicFormula, SymbolicFormula>();
      for (Pair<String, SymbolicFormula[]> p : predinfo.allFunctions) {
        SymbolicFormula[] args =
          getInstantiatedAt(p.getSecond(), symbSsa, cache);
        if (symbSsa.getIndex(p.getFirst(), args) < 0) {
          symbSsa.setIndex(p.getFirst(), args, 1);
        }
      }
    }
  
    List<SymbolicFormula> importantPreds = predinfo.predicateNames;
    CPAMain.logManager.log(Level.ALL, 
        "IMPORTANT SYMBOLS (", importantPreds.size(), "): ",
        importantPreds);
    
    // instantiate the definitions with the right SSA
    SymbolicFormula predDef = smgr.instantiate(predinfo.predicateDefinition, symbSsa);

    // the formula is (absFormula & symbFormula & predDef)
    final SymbolicFormula fm = smgr.makeAnd( 
        smgr.makeAnd(absFormula, symbFormula), predDef);
    
    CPAMain.logManager.log(Level.ALL, "DEBUG_2",
        "COMPUTING ALL-SMT ON FORMULA: ", fm);

    Pair<SymbolicFormula, List<SymbolicFormula>> absKey =
      new Pair<SymbolicFormula, List<SymbolicFormula>>(fm, importantPreds);
    AbstractFormula result;
    if (useCache && abstractionCache.containsKey(absKey)) {
      ++stats.numCallsAbstractionCached;
      result = abstractionCache.get(absKey);
      
      CPAMain.logManager.log(Level.ALL, "Abstraction was cached, result is", result);
    } else {
      // get the environment from the manager - this is unique, it is the
      // environment in which all terms are created
   
      AllSatCallback allSatCallback = new AllSatCallback();
      long msatSolveStartTime = System.currentTimeMillis();
      final int numModels = thmProver.allSat(fm, importantPreds, allSatCallback);
      long msatSolveEndTime = System.currentTimeMillis();

      assert(numModels != -1);  // msat_all_sat returns -1 on error

      if (numModels == -2) {
        // formula has infinite number of models
        result = amgr.makeTrue();
      } else {
        result = allSatCallback.getBDD();
      }

      if (useCache) {
        abstractionCache.put(absKey, result);
      }
      
      // update statistics
      long bddTime       = allSatCallback.totalTime;
      long msatSolveTime = (msatSolveEndTime - msatSolveStartTime) - bddTime;

      stats.abstractionMathsatSolveTime += msatSolveTime;
      stats.abstractionBddTime          += bddTime;
      startTime += bddTime; // do not count BDD creation time

      stats.abstractionMaxBddTime =
        Math.max(bddTime, stats.abstractionMaxBddTime);
      stats.abstractionMaxMathsatSolveTime =
        Math.max(msatSolveTime, stats.abstractionMaxMathsatSolveTime);

      // TODO dump hard abst
      if (msatSolveTime > 10000 && dumpHardAbstractions) {
        // we want to dump "hard" problems...
        MathsatAbstractionPrinter absPrinter = new MathsatAbstractionPrinter(mmgr.getMsatEnv(), "abs");
        absPrinter.printMsatFormat(absFormula, symbFormula, predDef, importantPreds);
        absPrinter.printNusmvFormat(absFormula, symbFormula, predDef, importantPreds);
        absPrinter.nextNum();
      }
      CPAMain.logManager.log(Level.ALL, "Abstraction computed, result is", result);
    }

    // update statistics
    long endTime = System.currentTimeMillis();
    long abstractionMsatTime = (endTime - startTime);
    stats.abstractionMathsatTime += abstractionMsatTime;
    stats.abstractionMaxMathsatTime =
      Math.max(abstractionMsatTime, stats.abstractionMaxMathsatTime);
    
    return result;
  }

  @Override
  public CounterexampleTraceInfo buildCounterexampleTrace(
      ArrayList<SymbPredAbsAbstractElement> abstractTrace) throws CPAException {

    long startTime = System.currentTimeMillis();
    stats.numCallsCexAnalysis++;

    CPAMain.logManager.log(Level.FINEST, "Building counterexample trace");

    List<SymbolicFormula> f = getFormulasForTrace(abstractTrace);

    CPAMain.logManager.log(Level.ALL, "Counterexample trace formulas:", f);
    
    boolean theoryCombinationNeeded = false;

    if (useDtc) {
      theoryCombinationNeeded = addBitwiseAxioms(f);
    }
    f = Collections.unmodifiableList(f);

    CPAMain.logManager.log(Level.FINEST, "Checking feasibility of counterexample trace");

    // now f is the DAG formula which is satisfiable iff there is a
    // concrete counterexample

    // create a working environment
    itpProver.init();

    long msatSolveTimeStart = System.currentTimeMillis();

    if (shortestTrace && getUsefulBlocks) {
      f = Collections.unmodifiableList(getUsefulBlocks(f, theoryCombinationNeeded, useSuffix, useZigZag));
    }

    List<T> itpGroupsIds = new ArrayList<T>(f.size());
    for (int i = 0; i < f.size(); i++) {
      itpGroupsIds.add(null);
    }
    
    boolean spurious;
    if (getUsefulBlocks || !shortestTrace) {
      // check all formulas in f at once
      
      for (int i = useSuffix ? f.size()-1 : 0;
          useSuffix ? i >= 0 : i < f.size(); i += useSuffix ? -1 : 1) {
          
        itpGroupsIds.set(i, itpProver.addFormula(f.get(i))); 
      }
      spurious = itpProver.isUnsat();
      
    } else {
      spurious = checkInfeasabilityOfShortestTrace(f, itpGroupsIds);
    }
    assert itpGroupsIds.size() == f.size();
    assert !itpGroupsIds.contains(null); // has to be filled completely
      
    long msatSolveTimeEnd = System.currentTimeMillis();
    long msatSolveTime = msatSolveTimeEnd - msatSolveTimeStart;

    CounterexampleTraceInfo info = new CounterexampleTraceInfo(spurious);

    CPAMain.logManager.log(Level.FINEST, "Counterexample trace is", (spurious ? "infeasible" : "feasible"));
    
    if (spurious) {
      // the counterexample is spurious. Extract the predicates from
      // the interpolants
      
      // how to partition the trace into (A, B) depends on whether
      // there are function calls involved or not: in general, A
      // is the trace from the entry point of the current function
      // to the current point, and B is everything else. To implement
      // this, we keep track of which function we are currently in.
      // if we don't want "well-scoped" predicates, A always starts at the beginning
      Deque<Integer> entryPoints = null;
      if (wellScopedPredicates) {
        entryPoints = new ArrayDeque<Integer>();
        entryPoints.push(0);
      }
      boolean foundPredicates = false;

      for (int i = 0; i < f.size()-1; ++i) {
        // last iteration is left out because B would be empty
        final int start_of_a = (wellScopedPredicates ? entryPoints.peek() : 0);
        SymbPredAbsAbstractElement e = abstractTrace.get(i);
        
        CPAMain.logManager.log(Level.ALL, "Looking for interpolant for formulas from",
            start_of_a, "to", i);
                
        msatSolveTimeStart = System.currentTimeMillis();
        SymbolicFormula itp = itpProver.getInterpolant(itpGroupsIds.subList(start_of_a, i+1));
        msatSolveTimeEnd = System.currentTimeMillis();
        msatSolveTime += msatSolveTimeEnd - msatSolveTimeStart;
                
        if (itp.isTrue() || itp.isFalse()) {
          CPAMain.logManager.log(Level.ALL, "For location", e.getAbstractionLocation(), "got no interpolant.");
        
        } else {
          foundPredicates = true;

          Collection<SymbolicFormula> atoms = mmgr.extractAtoms(itp, true, splitItpAtoms, false);
          assert !atoms.isEmpty();
          Set<Predicate> preds = buildPredicates(atoms);
          info.addPredicatesForRefinement(e, preds);

          CPAMain.logManager.log(Level.ALL, "For location", e.getAbstractionLocation(), "got:",
              "interpolant", itp,
              "atoms ", atoms,
              "predicates", preds);
        
        }

        // TODO the following code relies on the fact that there is always an abstraction on function call and return
        
        // If we are entering or exiting a function, update the stack
        // of entry points
        // TODO checking if the abstraction node is a new function
        if (wellScopedPredicates && e.getAbstractionLocation() instanceof CFAFunctionDefinitionNode) {
          entryPoints.push(i);
        }
        // TODO check we are returning from a function
        if (wellScopedPredicates && e.getAbstractionLocation().getEnteringSummaryEdge() != null) {
          entryPoints.pop();
        }
      }
      
      if (!foundPredicates) {
        throw new RefinementFailedException(RefinementFailedException.Reason.InterpolationFailed, null);
      }

    } else {
      // this is a real bug, notify the user
      ConcreteTraceFunctionCalls cf = new ConcreteTraceFunctionCalls();
      for (SymbPredAbsAbstractElement e : abstractTrace) {
        cf.add(e.getAbstractionLocation().getFunctionName());
      }
      info.setConcreteTrace(cf);
      
      // TODO - reconstruct counterexample
      // For now, we dump the asserted formula to a user-specified file
      dumpFormulasToFile(f);
    }

    itpProver.reset();

    // update stats
    long endTime = System.currentTimeMillis();
    long totTime = endTime - startTime;
    stats.cexAnalysisTime += totTime;
    stats.cexAnalysisMaxTime = Math.max(totTime, stats.cexAnalysisMaxTime);
    stats.cexAnalysisMathsatTime += msatSolveTime;
    stats.cexAnalysisMaxMathsatTime =
      Math.max(msatSolveTime, stats.cexAnalysisMaxMathsatTime);

    CPAMain.logManager.log(Level.ALL, "Counterexample information:", info);

    return info;
  }

  private void dumpFormulasToFile(List<SymbolicFormula> f) {
    String cexFile = CPAMain.cpaConfig.getProperty("cpas.symbpredabs.refinement.msatCexFile");
    if (cexFile != null) {
      String path = CPAMain.cpaConfig.getProperty("output.path") + cexFile;
      try {
        SymbolicFormula t = smgr.makeTrue();
        for (SymbolicFormula fm : f) {
          t = smgr.makeAnd(t, fm);
        }
        String msatRepr = mmgr.dumpFormula(t);

        PrintWriter pw = new PrintWriter(new File(path));
        pw.println(msatRepr);
        pw.close();
      } catch (FileNotFoundException e) {
        CPAMain.logManager.log(Level.WARNING,
            "Failed to save msat Counterexample to file ", path,
            " (", e.getMessage(), ")");
      }
    }
  }

  private List<SymbolicFormula> getFormulasForTrace(
      ArrayList<SymbPredAbsAbstractElement> abstractTrace) {

    // create the DAG formula corresponding to the abstract trace. We create
    // n formulas, one per interpolation group
    SSAMap ssa = null;

    List<SymbolicFormula> f = new ArrayList<SymbolicFormula>(abstractTrace.size()-1);

    for (int i = 0; i < abstractTrace.size(); ++i) {
      SymbPredAbsAbstractElement e = abstractTrace.get(i);
      // TODO here we take the formula from the abstract element
      PathFormula p = getInitSymbolicFormula(e.getInitAbstractionFormula(), (ssa == null));
      SSAMap newSsa;
      
      if (ssa != null) {
        p = smgr.shift(p.getSymbolicFormula(), ssa);
        newSsa = p.getSsa();
        newSsa.update(ssa);
      } else {
        newSsa = p.getSsa();
      }
      f.add(p.getSymbolicFormula());
      ssa = newSsa;

    }
    return f;
  }

  /**
   * Looks for uninterpreted functions in the trace formulas and adds bitwise
   * axioms for them to the last trace formula. Returns true if an UF was found.
   * @param mgr
   * @param traceFormulas
   * @return
   */
  private boolean addBitwiseAxioms(List<SymbolicFormula> traceFormulas) {

    boolean foundUninterpretedFunction = false;
    
    SymbolicFormula bitwiseAxioms = smgr.makeTrue();
    
    for (SymbolicFormula fm : traceFormulas) {
      boolean hasUf = mmgr.hasUninterpretedFunctions((MathsatSymbolicFormula)fm);
      if (hasUf) {
        foundUninterpretedFunction = true;  

        if (useBitwiseAxioms) {
          SymbolicFormula a = mmgr.getBitwiseAxioms((MathsatSymbolicFormula)fm);
          bitwiseAxioms = smgr.makeAnd(bitwiseAxioms, a);
        } else {
          // do not need to check all formulas, one with UF is enough
          break;
        }
      }
    }
    
    if (useBitwiseAxioms && foundUninterpretedFunction) {
      CPAMain.logManager.log(Level.ALL, "DEBUG_3", "ADDING BITWISE AXIOMS TO THE",
          "LAST GROUP: ", bitwiseAxioms);
      traceFormulas.set(traceFormulas.size()-1, smgr.makeAnd(traceFormulas.get(traceFormulas.size()-1), bitwiseAxioms));
    }
    return foundUninterpretedFunction;
  }
  
  private boolean checkInfeasabilityOfShortestTrace(List<SymbolicFormula> traceFormulas,
                                                    List<T> itpGroupsIds) {
      Boolean tmpSpurious = null;
      
      if (useZigZag) {
        int e = traceFormulas.size()-1;
        int s = 0;
        boolean fromStart = false;
        while (s <= e) {
          int i = fromStart ? s : e;
          if (fromStart) s++;
          else e--;
          fromStart = !fromStart;
          
          tmpSpurious = null;
          SymbolicFormula fm = traceFormulas.get(i);
          itpGroupsIds.set(i, itpProver.addFormula(fm));
          if (!fm.isTrue()) {
            if (itpProver.isUnsat()) {
              tmpSpurious = Boolean.TRUE;
              for (int j = s; j <= e; ++j) {
                itpGroupsIds.set(j, itpProver.addFormula(traceFormulas.get(j)));
              }
              break;
            } else {
              tmpSpurious = Boolean.FALSE;
            }
          }
        }
        
      } else {
        for (int i = useSuffix ? traceFormulas.size()-1 : 0;
              useSuffix ? i >= 0 : i < traceFormulas.size(); i += useSuffix ? -1 : 1) {
          
          tmpSpurious = null;
          SymbolicFormula fm = traceFormulas.get(i);
          itpGroupsIds.set(i, itpProver.addFormula(fm));
          if (!fm.isTrue()) {
            if (itpProver.isUnsat()) {
              tmpSpurious = Boolean.TRUE;
              // we need to add the other formulas to the itpProver
              // anyway, so it can setup its internal state properly
              for (int j = i+(useSuffix ? -1 : 1);
                  useSuffix ? j >= 0 : j < traceFormulas.size();
                  j += useSuffix ? -1 : 1) {
                itpGroupsIds.set(j, itpProver.addFormula(traceFormulas.get(j)));
              }
              break;
            } else {
              tmpSpurious = Boolean.FALSE;
            }
          }
        }
      }
      
      return (tmpSpurious == null) ? itpProver.isUnsat() : tmpSpurious;
  }

  private PathFormula getInitSymbolicFormula(PathFormula pf, boolean replace) {
    SSAMap ssa = new SSAMap();
    SymbolicFormula f = smgr.makeFalse();
    Pair<Pair<SymbolicFormula, SymbolicFormula>, SSAMap> mp =
      smgr.mergeSSAMaps(ssa, pf.getSsa(), false);
    SymbolicFormula curf = pf.getSymbolicFormula();
    // TODO modified if
    if (replace) {
      curf = mmgr.replaceAssignments((MathsatSymbolicFormula)curf);
    }
    f = smgr.makeAnd(f, mp.getFirst().getFirst());
    curf = smgr.makeAnd(curf, mp.getFirst().getSecond());
    f = smgr.makeOr(f, curf);
    ssa = mp.getSecond();
    return new PathFormula(f,ssa);
  }

  
  // generates the predicates corresponding to the given atoms, which were
  // extracted from the interpolant
  private Set<Predicate> buildPredicates(Collection<SymbolicFormula> atoms) {
    long dstenv = mmgr.getMsatEnv();
    Set<Predicate> ret = new HashSet<Predicate>();
    for (SymbolicFormula atom : atoms) {
      long tt = ((MathsatSymbolicFormula)atom).getTerm();
      long d = mathsat.api.msat_declare_variable(dstenv,
          "\"PRED" + mathsat.api.msat_term_repr(tt) + "\"",
          mathsat.api.MSAT_BOOL);
      long var = mathsat.api.msat_make_variable(dstenv, d);

      assert(!mathsat.api.MSAT_ERROR_TERM(var));

      ret.add(makePredicate(new MathsatSymbolicFormula(var), atom));
    }
    return ret;
  }

  private List<SymbolicFormula> getUsefulBlocks(List<SymbolicFormula> f,
      boolean theoryCombinationNeeded, boolean suffixTrace, boolean zigZag) {
    long gubStart = System.currentTimeMillis();
    
    // try to find a minimal-unsatisfiable-core of the trace (as Blast does)

    thmProver.init(TheoremProver.COUNTEREXAMPLE_ANALYSIS);

    CPAMain.logManager.log(Level.ALL, "DEBUG_1", "Calling getUsefulBlocks on path",
        "of length:", f.size());

    SymbolicFormula trueFormula = smgr.makeTrue();
    SymbolicFormula[] needed = new SymbolicFormula[f.size()];
    for (int i = 0; i < needed.length; ++i) {
      needed[i] = trueFormula;
    }
    int pos = suffixTrace ? f.size()-1 : 0;
    int incr = suffixTrace ? -1 : 1;
    int toPop = 0;

    while (true) {
      boolean consistent = true;
      // 1. assert all the needed constraints
      for (int i = 0; i < needed.length; ++i) {
        if (!needed[i].isTrue()) {
          thmProver.push(needed[i]);
          ++toPop;
        }
      }
      // 2. if needed is inconsistent, then return it
      if (thmProver.isUnsat(trueFormula)) {
        f = Arrays.asList(needed);
        break;
      }
      // 3. otherwise, assert one block at a time, until we get an
      // inconsistency
      if (zigZag) {
        int s = 0;
        int e = f.size()-1;
        boolean fromStart = false;
        while (true) {
          int i = fromStart ? s : e;
          if (fromStart) ++s;
          else --e;
          fromStart = !fromStart;

          SymbolicFormula t = f.get(i);
          thmProver.push(t);
          ++toPop;
          if (thmProver.isUnsat(trueFormula)) {
            // add this block to the needed ones, and repeat
            needed[i] = t;
            CPAMain.logManager.log(Level.ALL, "DEBUG_1",
                "Found needed block: ", i, ", term: ", t);
            // pop all
            while (toPop > 0) {
              --toPop;
              thmProver.pop();
            }
            // and go to the next iteration of the while loop
            consistent = false;
            break;
          }

          if (e < s) break;
        }
      } else {
        for (int i = pos; suffixTrace ? i >= 0 : i < f.size();
        i += incr) {
          SymbolicFormula t = f.get(i);
          thmProver.push(t);
          ++toPop;
          if (thmProver.isUnsat(trueFormula)) {
            // add this block to the needed ones, and repeat
            needed[i] = t;
            CPAMain.logManager.log(Level.ALL, "DEBUG_1",
                "Found needed block: ", i, ", term: ", t);
            // pop all
            while (toPop > 0) {
              --toPop;
              thmProver.pop();
            }
            // and go to the next iteration of the while loop
            consistent = false;
            break;
          }
        }
      }
      if (consistent) {
        // if we get here, the trace is consistent:
        // this is a real counterexample!
        break;
      }
    }

    while (toPop > 0) {
      --toPop;
      thmProver.pop();
    }

    thmProver.reset();

    CPAMain.logManager.log(Level.ALL, "DEBUG_1", "Done getUsefulBlocks");

    long gubEnd = System.currentTimeMillis();
    stats.cexAnalysisGetUsefulBlocksTime += gubEnd - gubStart;
    stats.cexAnalysisGetUsefulBlocksMaxTime = Math.max(
        stats.cexAnalysisGetUsefulBlocksMaxTime, gubEnd - gubStart);
    
    return f;
  }

}
