/* Extracted from interp.c:29737 (function addressCouldBeObj). */

/* SpurMemoryManager>>#addressCouldBeObj: */

sqInt addressCouldBeObj(sqInt address) {
  return ((address & (BaseHeaderSize - 1)) == 0) &&
         ((oopisGreaterThanOrEqualTo(address, oldSpaceStart)
               ? oopisLessThan(address, endOfMemory)
               : (oopisGreaterThanOrEqualToandLessThan(address, ((eden).start),
                                                       freeStart)) ||
                     ((oopisGreaterThanOrEqualToandLessThan(
                          address, ((pastSpace).start), pastSpaceStart)) ||
                      ((gcPhaseInProgress == ScavengeInProgress) &&
                       (oopisGreaterThanOrEqualToandLessThan(
                           address, ((futureSpace).start),
                           futureSurvivorStart))))));
}