/* Extracted from interp.c:51521 (function copiedValueCountOfClosure). */

/*	for Cogit */

/* StackInterpreter>>#copiedValueCountOfClosure: */

sqInt copiedValueCountOfClosure(sqInt closureObj) {
  usqInt numSlots;

  return (
      ((((fetchPointerofObject(ClosureStartPCIndex, closureObj))) & 7) == 1)
          ? (/* begin copiedValueCountOfVanillaClosure: */
             assert(isVanillaBlockClosure(closureObj)),
             ((
               assert((classIndexOf(closureObj)) >
                      (isForwardedObjectClassIndexPun())),
               numSlotsOf(closureObj))) -
                 ClosureFirstCopiedValueIndex)
          : (/* begin copiedValueCountOfFullClosure: */
             assert(!((isVanillaBlockClosure(closureObj)))),
             ((
               assert((classIndexOf(closureObj)) >
                      (isForwardedObjectClassIndexPun())),
               numSlotsOf(closureObj))) -
                 FullClosureFirstCopiedValueIndex));
}