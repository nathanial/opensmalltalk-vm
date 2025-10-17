/* Extracted from interp.c:37418 (function isInPastSpace). */

/* SpurMemoryManager>>#isInPastSpace: */

static sqInt isInPastSpace(sqInt address) {
  return oopisGreaterThanOrEqualToandLessThan(address, ((pastSpace).start),
                                              pastSpaceStart);
}