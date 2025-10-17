/* Extracted from interp.c:31746 (function beRootIfOld). */

/*	If this object is old, mark it as a root (because a new object
        may be stored into it). */

/* SpurMemoryManager>>#beRootIfOld: */

void beRootIfOld(sqInt oop) {
  /* begin isOldObject: */
  assert(isNonImmediate(oop));
  if (oopisGreaterThanOrEqualTo(oop, oldSpaceStart)) {
    /* begin possibleRootStoreInto: */
    if (!((byteAt((void *)(oop + (formatFieldByteOffset())))) &
          (1U << (rememberedBitByteShift())))) {
      remember(oop);
    }
  }
}