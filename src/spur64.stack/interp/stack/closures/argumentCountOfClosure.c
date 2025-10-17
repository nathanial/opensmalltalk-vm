/* Extracted from interp.c:50106 (function argumentCountOfClosure). */

/*	for Cogit */

/* StackInterpreter>>#argumentCountOfClosure: */

sqInt argumentCountOfClosure(sqInt closurePointer) {
  sqInt oop;

  /* begin quickFetchInteger:ofObject: */
  oop =
      fetchPointerofObject(ClosureNumArgsIndex, closurePointer);
  assert((((oop) & 7) == 1));
  return (oop >> 3);
}