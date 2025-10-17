/* Extracted from interp.c:28894 (function
 * scavengeFutureSurvivorSpaceStartingAt). */

/*	scavengeFutureSurvivorSpaceStartingAt: does a depth-first traversal of
   the new objects starting at the one at initialAddress in futureSurvivorSpace.
 */

/* SpurGenerationScavenger>>#scavengeFutureSurvivorSpaceStartingAt: */

static NoDbgRegParms void
scavengeFutureSurvivorSpaceStartingAt(sqInt initialAddress) {
  usqInt numSlots;
  sqInt obj;
  usqInt ptr;

  ptr = initialAddress;
  while (ptr < futureSurvivorStart) {
    /* begin objectStartingAt: */
    numSlots = byteAt((void *)(ptr + (numSlotsFieldByteOffset())));
    obj = (numSlots == (numSlotsMask()) ? ptr + BaseHeaderSize : ptr);
    ptr = addressAfter(obj);
    ((void)(scavengeReferentsOf(obj)));
  }
}