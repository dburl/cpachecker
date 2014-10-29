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
package org.sosy_lab.cpachecker.util.predicates.interfaces.view.replacing;

import java.util.List;

import org.sosy_lab.common.Appender;
import org.sosy_lab.cpachecker.util.predicates.interfaces.BitvectorFormula;
import org.sosy_lab.cpachecker.util.predicates.interfaces.BitvectorFormulaManager;
import org.sosy_lab.cpachecker.util.predicates.interfaces.BooleanFormula;
import org.sosy_lab.cpachecker.util.predicates.interfaces.BooleanFormulaManager;
import org.sosy_lab.cpachecker.util.predicates.interfaces.Formula;
import org.sosy_lab.cpachecker.util.predicates.interfaces.FormulaManager;
import org.sosy_lab.cpachecker.util.predicates.interfaces.FormulaType;
import org.sosy_lab.cpachecker.util.predicates.interfaces.FunctionFormulaManager;
import org.sosy_lab.cpachecker.util.predicates.interfaces.NumeralFormula;
import org.sosy_lab.cpachecker.util.predicates.interfaces.NumeralFormula.IntegerFormula;
import org.sosy_lab.cpachecker.util.predicates.interfaces.NumeralFormula.RationalFormula;
import org.sosy_lab.cpachecker.util.predicates.interfaces.NumeralFormulaManager;
import org.sosy_lab.cpachecker.util.predicates.interfaces.QuantifiedFormulaManager;
import org.sosy_lab.cpachecker.util.predicates.interfaces.UnsafeFormulaManager;
import org.sosy_lab.cpachecker.util.predicates.interfaces.view.FormulaManagerView.Theory;

import com.google.common.base.Function;
import com.google.common.base.Functions;
import com.google.common.collect.Lists;


public class ReplacingFormulaManager implements FormulaManager {

  private final FormulaManager rawFormulaManager;
  private final BitvectorFormulaManager bitvectorTheory;
  private final boolean replacedBitvectorTheory;
  private final boolean replacedRationalTheory;
  private final FunctionFormulaManager functionTheory;
  private final BooleanFormulaManager booleanTheory;
  private final UnsafeFormulaManager unsafeManager;
  private final QuantifiedFormulaManager quantifiedFormulaManager;

  public ReplacingFormulaManager(
      FormulaManager rawFormulaManager,
      final Theory bitvectorReplacement,
      final boolean ignoreExtractConcat) {
    this.rawFormulaManager = rawFormulaManager;
    this.quantifiedFormulaManager = rawFormulaManager.getQuantifiedFormulaManager();
    replacedRationalTheory = false;

    final Function<FormulaType<?>, FormulaType<?>> unwrapTypes;

    if (bitvectorReplacement == Theory.BITVECTOR) {
      replacedBitvectorTheory = false;
      bitvectorTheory = rawFormulaManager.getBitvectorFormulaManager();
      unwrapTypes = Functions.identity();

    } else {
      replacedBitvectorTheory = true;
      final FormulaType<?> replacementType;
      switch (bitvectorReplacement) {
      case INTEGER:
        bitvectorTheory = new ReplaceBitvectorWithNumeralAndFunctionTheory<>(
            this,
            rawFormulaManager.getIntegerFormulaManager(),
            rawFormulaManager.getFunctionFormulaManager(),
            ignoreExtractConcat);
        replacementType = rawFormulaManager.getIntegerFormulaManager().getFormulaType();
        break;
      case RATIONAL:
        bitvectorTheory = new ReplaceBitvectorWithNumeralAndFunctionTheory<>(
            this,
            rawFormulaManager.getRationalFormulaManager(),
            rawFormulaManager.getFunctionFormulaManager(),
            ignoreExtractConcat);
        replacementType = rawFormulaManager.getRationalFormulaManager().getFormulaType();
        break;
      default:
        throw new AssertionError("Unknown theory " + bitvectorReplacement);
      }

      unwrapTypes = new
          Function<FormulaType<?>, FormulaType<?>>() {
            @Override
            public FormulaType<?> apply(FormulaType<?> pArg0) {
              if (pArg0.isBitvectorType()) {
                return replacementType;
              }

              return pArg0;
            }};

    }

    functionTheory =
        new ReplaceHelperFunctionFormulaManager(
            this,
            rawFormulaManager.getFunctionFormulaManager(),
            unwrapTypes);
    booleanTheory =
        new ReplaceHelperBooleanFormulaManager(
            this,
            rawFormulaManager.getBooleanFormulaManager());
    unsafeManager =
        new ReplaceUnsafeFormulaManager(
            this,
            rawFormulaManager.getUnsafeFormulaManager(),
            unwrapTypes);
  }

  @SuppressWarnings("unchecked")
  public <T1 extends Formula, T2 extends Formula> T1 simpleWrap(FormulaType<T1> type, T2 toWrap) {
    Formula f;
    if (type.isBitvectorType()) {
      f = new WrappingBitvectorFormula<>((FormulaType<BitvectorFormula>)type, toWrap);
    } else if (type.isIntegerType()) {
      f = new WrappingIntegerFormula<>((FormulaType<IntegerFormula>)type, toWrap);
    } else if (type.isRationalType()) {
      f = new WrappingRationalFormula<>((FormulaType<RationalFormula>)type, toWrap);
    } else if (type.isBooleanType()) {
      f = new WrappingBooleanFormula<>((FormulaType<BooleanFormula>)type, toWrap);
    } else {
      throw new IllegalArgumentException("cant wrap this type");
    }

    return (T1) f;
  }

  @SuppressWarnings("unchecked")
  public <T1 extends Formula, T2 extends Formula> T1 wrap(FormulaType<T1> targetType, T2 toWrap) {
    FormulaType<T2> toWrapType = rawFormulaManager.getFormulaType(toWrap);

    if (replacedBitvectorTheory && targetType.isBitvectorType()
        || replacedRationalTheory && targetType.isRationalType()) {
      return simpleWrap(targetType, toWrap);
    } else if (toWrapType.equals(targetType)) {
      return (T1) toWrap;
    } else {
      throw new IllegalArgumentException("invalid wrap call");
    }
  }

  public Formula unwrap(Formula f) {
    if (f instanceof WrappingFormula<?, ?>) {
      return ((WrappingFormula<?, ?>)f).getWrapped();
    } else {
      return f;
    }
  }

  List<Formula> unwrap(List<? extends Formula> f) {
    return Lists.transform(f, new Function<Formula, Formula>() {
      @Override
      public Formula apply(Formula pInput) {
        return unwrap(pInput);
      }
    });
  }

  @Override
  public NumeralFormulaManager<IntegerFormula, IntegerFormula> getIntegerFormulaManager() {
    return rawFormulaManager.getIntegerFormulaManager();
  }

  @Override
  public NumeralFormulaManager<NumeralFormula, RationalFormula> getRationalFormulaManager() {
    return rawFormulaManager.getRationalFormulaManager();
  }

  @Override
  public BooleanFormulaManager getBooleanFormulaManager() {
    return booleanTheory;
  }

  @Override
  public BitvectorFormulaManager getBitvectorFormulaManager() {
    return bitvectorTheory;
  }

  @Override
  public FunctionFormulaManager getFunctionFormulaManager() {
    return functionTheory;
  }

  @Override
  public UnsafeFormulaManager getUnsafeFormulaManager() {
    return unsafeManager;
  }

  @SuppressWarnings("unchecked")
  @Override
  public <T extends Formula> FormulaType<T> getFormulaType(T pFormula) {
    FormulaType<?> t;
    if (pFormula instanceof WrappingFormula<?, ?>) {
      WrappingFormula<?, ?> castFormula = (WrappingFormula<?, ?>)pFormula;
      t = castFormula.getType();
    } else {
      t = rawFormulaManager.getFormulaType(pFormula);
    }

    return (FormulaType<T>) t;
  }

  @Override
  public BooleanFormula parse(String pS) throws IllegalArgumentException {
    return rawFormulaManager.parse(pS);
  }

  @Override
  public Appender dumpFormula(Formula pT) {
    if (pT instanceof WrappingFormula<?, ?>) {
      pT = unwrap(pT);
    }

    return rawFormulaManager.dumpFormula(pT);
  }

  @Override
  public String getVersion() {
    return rawFormulaManager.getVersion();
  }

  @Override
  public QuantifiedFormulaManager getQuantifiedFormulaManager() {
    return quantifiedFormulaManager;
  }

}
