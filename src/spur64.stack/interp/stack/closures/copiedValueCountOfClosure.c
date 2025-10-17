/* Extracted from interp.c:51521 (function copiedValueCountOfClosure). */

/*	for Cogit */

/* StackInterpreter>>#copiedValueCountOfClosure: */

sqInt copiedValueCountOfClosure(sqInt closureObj) {
  usqInt numSlots;

  return (
      ((((fetchPointerofObject(ClosureStartPCIndex, closureObj))) & 7) == 1)
          ? (/* begin copiedValueCountOfVanillaClosure: */
             assert(isVanillaBlockClosure(closureObj)),
             ((/* begin numSlotsOf: */
               assert((classIndexOf(closureObj)) >
                      (isForwardedObjectClassIndexPun())),
               (((numSlots = byteAt(
                      (void *)(closureObj + (numSlotsFieldByteOffset()))))) ==
                        (numSlotsMask())
                    ? ((((usqInt)((
                          (sqInt)((usqInt)((longAt(
                                      (void *)(closureObj - BaseHeaderSize))))
                                  << 8)))))) >>
                          8
                    : numSlots))) -
                 ClosureFirstCopiedValueIndex)
          : (/* begin copiedValueCountOfFullClosure: */
             assert(!((isVanillaBlockClosure(closureObj)))),
             ((/* begin numSlotsOf: */
               assert((classIndexOf(closureObj)) >
                      (isForwardedObjectClassIndexPun())),
               (((numSlots = byteAt(
                      (void *)(closureObj + (numSlotsFieldByteOffset()))))) ==
                        (numSlotsMask())
                    ? ((((usqInt)((
                          (sqInt)((usqInt)((longAt(
                                      (void *)(closureObj - BaseHeaderSize))))
                                  << 8)))))) >>
                          8
                    : numSlots))) -
                 FullClosureFirstCopiedValueIndex));
}