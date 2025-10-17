/* Extracted from interp.c:37364 (function isInFutureSpace). */

/* SpurMemoryManager>>#isInFutureSpace: */

static NoDbgRegParms sqInt isInFutureSpace(sqInt address) {
  return oopisGreaterThanOrEqualToandLessThan(address, ((futureSpace).start),
                                              futureSurvivorStart);
}