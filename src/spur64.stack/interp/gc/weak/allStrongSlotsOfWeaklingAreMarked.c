/* Extracted from interp.c:31074 (function allStrongSlotsOfWeaklingAreMarked).
 */

/*	N.B. generateToByDoLimitExpression:negative:on: guards against
   (unsigned)0
        - 1 going +ve
 */

/* SpurMemoryManager>>#allStrongSlotsOfWeaklingAreMarked: */

static sqInt allStrongSlotsOfWeaklingAreMarked(sqInt aWeakling) {
  sqInt i;
  sqInt referent;
  sqInt toDoLimit;

  toDoLimit = (numStrongSlotsOfWeakling(aWeakling)) - 1;
  for (i = 0; i <= toDoLimit; i += 1) {
    referent = fetchPointerofObject(i, aWeakling);
    if ((!(referent & (tagMask())))) {
      if (!((byteAt((void *)(referent + (markBitsByteOffset())))) &
            (1U << (markedBitByteShift())))) {
        return 0;
      }
    }
  }
  return 1;
}