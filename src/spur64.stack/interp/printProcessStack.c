/* Extracted from interp.c:60810 (function printProcessStack). */

/*	useful for VM debugging */

/* StackInterpreter>>#printProcessStack: */

void printProcessStack(sqInt aProcess) {
  sqInt ctx;
  sqInt tagBits;

  cr();
  printNameOfClasscount(/* fetchClassOf: */
                        ((tagBits = aProcess & (tagMask()))
                             ? longAt((void *)((classTableFirstPage +
                                                BaseHeaderSize) +
                                               ((((usqInt)(tagBits)
                                                  << (shiftForWord()))))))
                             : fetchClassOfNonImm(aProcess)),
                        5);

  /* begin space */
  printChar(' ');
  printHex(aProcess);
  print(" priority ");
  printNum(quickFetchIntegerofObject(PriorityIndex, aProcess));
  cr();
  ctx = followFieldofObject(SuspendedContextIndex, aProcess);
  if (!(ctx == nilObj)) {
    printCallStackOfcurrentFP(ctx, framePointer);
  }
}