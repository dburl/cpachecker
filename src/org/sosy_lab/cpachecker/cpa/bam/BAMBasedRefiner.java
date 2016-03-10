/*
 *  CPAchecker is a tool for configurable software verification.
 *  This file is part of CPAchecker.
 *
 *  Copyright (C) 2007-2014  Dirk Beyer
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
package org.sosy_lab.cpachecker.cpa.bam;

import static com.google.common.base.Preconditions.checkArgument;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

import org.sosy_lab.common.configuration.InvalidConfigurationException;
import org.sosy_lab.common.time.Timer;
import org.sosy_lab.cpachecker.core.counterexample.CounterexampleInfo;
import org.sosy_lab.cpachecker.core.interfaces.ConfigurableProgramAnalysis;
import org.sosy_lab.cpachecker.core.interfaces.Refiner;
import org.sosy_lab.cpachecker.core.interfaces.Statistics;
import org.sosy_lab.cpachecker.core.interfaces.StatisticsProvider;
import org.sosy_lab.cpachecker.cpa.arg.ARGBasedRefiner;
import org.sosy_lab.cpachecker.cpa.arg.ARGPath;
import org.sosy_lab.cpachecker.cpa.arg.ARGReachedSet;
import org.sosy_lab.cpachecker.cpa.arg.ARGState;
import org.sosy_lab.cpachecker.cpa.arg.ARGUtils;
import org.sosy_lab.cpachecker.cpa.arg.AbstractARGBasedRefiner;
import org.sosy_lab.cpachecker.exceptions.CPAException;
import org.sosy_lab.cpachecker.exceptions.CPATransferException;

/**
 * This is an extension of {@link AbstractARGBasedRefiner} that takes care of
 * flattening the ARG before calling
 * {@link ARGBasedRefiner#performRefinement(ARGReachedSet, ARGPath)}.
 *
 * Warning: Although the ARG is flattened at this point, the elements in it have
 * not been expanded due to performance reasons.
 */
public final class BAMBasedRefiner extends AbstractARGBasedRefiner implements StatisticsProvider {

  final Timer computePathTimer = new Timer();
  final Timer computeSubtreeTimer = new Timer();
  final Timer computeCounterexampleTimer = new Timer();
  final Timer removeCachedSubtreeTimer = new Timer();

  private final ARGBasedRefiner refiner;
  private final BAMCPA bamCpa;
  private final Map<ARGState, ARGState> subgraphStatesToReachedState = new HashMap<>();
  private ARGState rootOfSubgraph = null;

  private BAMBasedRefiner(ConfigurableProgramAnalysis pCpa, ARGBasedRefiner pRefiner)
      throws InvalidConfigurationException {
    super(pCpa);

    bamCpa = (BAMCPA)pCpa;
    bamCpa.getStatistics().addRefiner(this);
    refiner = pRefiner;
  }

  /**
   * Create a {@link BAMBasedRefiner} instance (which is also a {@link Refiner} instance)
   * wrapping a {@link ARGBasedRefiner} instance.
   */
  public static BAMBasedRefiner forARGBasedRefiner(
      final ARGBasedRefiner pRefiner, final ConfigurableProgramAnalysis pCpa)
      throws InvalidConfigurationException {
    checkArgument(
        !(pRefiner instanceof Refiner),
        "ARGBasedRefiners may not implement Refiner, choose between these two!");
    return new BAMBasedRefiner(pCpa, pRefiner);
  }

  @Override
  protected final CounterexampleInfo performRefinement(ARGReachedSet pReached, ARGPath pPath) throws CPAException, InterruptedException {
    checkArgument(!(pReached instanceof BAMReachedSet),
        "Wrapping of BAM-based refiners inside BAM-based refiners is not allowed.");
    assert pPath == null || pPath.size() > 0;

    if (pPath == null) {
      // The counter-example-path could not be constructed, because of missing blocks (aka "holes").
      // We directly return SPURIOUS and let the CPA-algorithm run again.
      // During the counter-example-path-building we already re-added the start-states of all blocks,
      // that lead to the missing block, to the waitlists of those blocks.
      // Thus missing blocks are analyzed and rebuild again in the next CPA-algorithm.
      return CounterexampleInfo.spurious();
    } else {
      // wrap the original reached-set to have a valid "view" on all reached states.
      pReached = new BAMReachedSet(bamCpa, pReached, pPath, subgraphStatesToReachedState, rootOfSubgraph, removeCachedSubtreeTimer);
      return refiner.performRefinement(pReached, pPath);
    }
  }

  @Override
  protected final ARGPath computePath(ARGState pLastElement, ARGReachedSet pReachedSet) throws InterruptedException, CPATransferException {
    assert pLastElement.isTarget();
    assert pReachedSet.asReachedSet().contains(pLastElement) : "targetState must be in mainReachedSet.";

    computePathTimer.start();
    try {
      computeSubtreeTimer.start();
      try {
        rootOfSubgraph = computeCounterexampleSubgraph(pLastElement, pReachedSet);
        if (rootOfSubgraph == BAMCEXSubgraphComputer.DUMMY_STATE_FOR_MISSING_BLOCK) {
          return null;
        }
      } finally {
        computeSubtreeTimer.stop();
      }

      computeCounterexampleTimer.start();
      try {
        // We assume, that every path in the subgraph reaches the target state. Thus we choose randomly.
        return ARGUtils.getRandomPath(rootOfSubgraph);
      } finally {
        computeCounterexampleTimer.stop();
      }
    } finally {
      computePathTimer.stop();
    }
  }

  //returns root of a subtree leading from the root element of the given reachedSet to the target state
  //subtree is represented using children and parents of ARGElements, where newTreeTarget is the ARGState
  //in the constructed subtree that represents target
  private ARGState computeCounterexampleSubgraph(ARGState target, ARGReachedSet reachedSet) {
    assert reachedSet.asReachedSet().contains(target);

    // cleanup old states from last refinement
    subgraphStatesToReachedState.clear();

    final BAMCEXSubgraphComputer cexSubgraphComputer = new BAMCEXSubgraphComputer(bamCpa, subgraphStatesToReachedState);
    return cexSubgraphComputer.computeCounterexampleSubgraph(target, reachedSet);
  }

  @Override
  public void collectStatistics(Collection<Statistics> pStatsCollection) {
    if (refiner instanceof StatisticsProvider) {
      ((StatisticsProvider) refiner).collectStatistics(pStatsCollection);
    }
  }

  @Override
  public String toString() {
    return refiner.toString();
  }
}