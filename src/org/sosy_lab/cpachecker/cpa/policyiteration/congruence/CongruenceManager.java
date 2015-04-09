package org.sosy_lab.cpachecker.cpa.policyiteration.congruence;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.sosy_lab.cpachecker.cpa.policyiteration.Template;
import org.sosy_lab.cpachecker.cpa.policyiteration.Template.Kind;
import org.sosy_lab.cpachecker.cpa.policyiteration.TemplateManager;
import org.sosy_lab.cpachecker.exceptions.CPATransferException;
import org.sosy_lab.cpachecker.exceptions.SolverException;
import org.sosy_lab.cpachecker.util.predicates.Solver;
import org.sosy_lab.cpachecker.util.predicates.interfaces.BitvectorFormula;
import org.sosy_lab.cpachecker.util.predicates.interfaces.BitvectorFormulaManager;
import org.sosy_lab.cpachecker.util.predicates.interfaces.BooleanFormula;
import org.sosy_lab.cpachecker.util.predicates.interfaces.BooleanFormulaManager;
import org.sosy_lab.cpachecker.util.predicates.interfaces.Formula;
import org.sosy_lab.cpachecker.util.predicates.interfaces.ProverEnvironment;
import org.sosy_lab.cpachecker.util.predicates.interfaces.view.FormulaManagerView;
import org.sosy_lab.cpachecker.util.predicates.pathformula.PathFormula;

import com.google.common.collect.Sets;

public class CongruenceManager {

  private final Solver solver;
  private final TemplateManager templateManager;
  private final BitvectorFormulaManager bvfmgr;
  private final FormulaManagerView fmgr;
  private final BooleanFormulaManager bfmgr;

  public CongruenceManager(Solver pSolver, TemplateManager pTemplateManager,
      FormulaManagerView pFmgr) {
    solver = pSolver;
    templateManager = pTemplateManager;
    fmgr = pFmgr;
    bvfmgr = fmgr.getBitvectorFormulaManager();
    bfmgr = fmgr.getBooleanFormulaManager();
  }

  public CongruenceState join(
      CongruenceState a,
      CongruenceState b
  ) {
    Map<Template, Congruence> abstraction = new HashMap<>();
    for (Template t : Sets.intersection(
        a.getAbstraction().keySet(), b.getAbstraction().keySet()
    )) {
      if (a.getAbstraction().get(t) == b.getAbstraction().get(t)) {
        abstraction.put(t, a.getAbstraction().get(t));
      }
    }
    return new CongruenceState(abstraction);
  }


  public CongruenceState performAbstraction(
      PathFormula p,
      BooleanFormula startConstraints,
      Set<Template> templates
  ) throws CPATransferException, InterruptedException {

    Map<Template, Congruence> abstraction = new HashMap<>();

    try (ProverEnvironment env = solver.newProverEnvironment()) {
      //noinspection ResultOfMethodCallIgnored
      env.push(p.getFormula());
      //noinspection ResultOfMethodCallIgnored
      env.push(startConstraints);

      for (Template template : templates) {
        if (!shouldUseTemplate(template)) {
          continue;
        }

        Formula formula = templateManager.toFormula(template, p);

        // Test odd <=> isEven is UNSAT.
        try {
          //noinspection ResultOfMethodCallIgnored
          env.push(fmgr.makeModularCongruence(formula, makeBv(formula, 0), 2));
          if (env.isUnsat()) {
            abstraction.put(template, Congruence.ODD);
            continue;
          }
        } finally {
          env.pop();
        }

        // Test even <=> isOdd is UNSAT.
        try {
          //noinspection ResultOfMethodCallIgnored
          env.push(
              fmgr.makeModularCongruence(formula, makeBv(formula, 1), 2));
          if (env.isUnsat()) {
            abstraction.put(template, Congruence.EVEN);
            continue;
          }
        } finally {
          env.pop();
        }
      }
    } catch (SolverException ex) {
      throw new CPATransferException("Solver exception: ", ex);
    }

    return new CongruenceState(abstraction);
  }

  public BooleanFormula toFormula(
      CongruenceState state,
      PathFormula ref) {
    Map<Template, Congruence> abstraction = state.getAbstraction();

    List<BooleanFormula> constraints = new ArrayList<>(abstraction.size());

    for (Entry<Template, Congruence> entry : abstraction.entrySet()) {
      Template template = entry.getKey();
      Congruence congr = entry.getValue();

      Formula formula = templateManager.toFormula(template, ref);
      Formula remainder;
      switch (congr) {
        case ODD:
          remainder = makeBv(formula, 1);
          break;
        case EVEN:
          remainder = makeBv(formula, 0);
          break;
        default:
          remainder = null;
          assert false : "Unexpected case";
      }

      constraints.add(fmgr.makeModularCongruence(formula, remainder, 2));
    }
    return bfmgr.and(constraints);
  }

  private boolean shouldUseTemplate(Template template) {
    return template.getType().getType().isIntegerType() &&
        (template.getKind() == Kind.UPPER_BOUND ||
            template.getKind() == Kind.SUM);
  }

  private Formula makeBv(Formula other, int value) {
    return bvfmgr.makeBitvector(
        bvfmgr.getLength((BitvectorFormula) other),
        value);
  }
}
