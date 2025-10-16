/* Extracted from interp.c:54716 (function homeMethodOf). */

/*	With the full block implementation a CompiledBlock's method is found
   along the chain through the last literal. See CompiledBlock>>#method. Answer
        the home mehtod, or nilObj if it cannot be found. */

/* StackInterpreter>>#homeMethodOf: */

sqInt homeMethodOf(sqInt aMethodOop) {
  sqInt lastLiteral;
  sqInt methodChain;

  assert(isOopCompiledMethod(aMethodOop));
  methodChain = aMethodOop;
  while (1) {
    lastLiteral = ultimateLiteralOf(methodChain);
    if (!(isOopCompiledMethod(lastLiteral))) {
      return methodChain;
    }
    if (!(isOopCompiledMethod(lastLiteral))) {
      return nilObj;
    }
    methodChain = lastLiteral;
  }
  return 0;
}