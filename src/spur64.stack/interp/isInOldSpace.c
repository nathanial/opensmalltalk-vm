/* Extracted from interp.c:37411 (function isInOldSpace). */

/* SpurMemoryManager>>#isInOldSpace: */

sqInt isInOldSpace(sqInt address) {
  return oopisGreaterThanOrEqualToandLessThan(address, oldSpaceStart,
                                              endOfMemory);
}