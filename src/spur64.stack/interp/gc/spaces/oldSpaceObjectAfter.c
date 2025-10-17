/* Extracted from interp.c:42086 (function oldSpaceObjectAfter). */

/*	useful for debugging */
/*	Object parsing.
        1. all objects have at least a word following the header, for a
   forwarding pointer. 2. objects with an overflow size have a preceeing word
   with a saturated slotSize. If the word following an object doesn't have a
   saturated size field it must be a single-header object. If the word following
        does have a saturated slotSize it must be the overflow size word. */

/* SpurMemoryManager>>#oldSpaceObjectAfter: */

sqInt oldSpaceObjectAfter(sqInt objOop) {
  sqInt followingWord;
  usqInt followingWordAddress;

  /* begin objectAfter:limit: */
  followingWordAddress = addressAfter(objOop);
  if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
    return endOfMemory;
  }
  followingWord = longAt((void *)(followingWordAddress));
  return ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                  (numSlotsMask())
              ? followingWordAddress + BaseHeaderSize
              : followingWordAddress);
}