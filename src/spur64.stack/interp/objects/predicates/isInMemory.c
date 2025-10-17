/* Extracted from interp.c:37374 (function isInMemory). */

/*	Answer if the given address is in ST object memory. */

/* SpurMemoryManager>>#isInMemory: */

sqInt isInMemory(sqInt address) {
  usqInt addressUsqInt;
  sqInt i;

  if (/* isInNewSpace: */
      (oopisLessThan(address, oldSpaceStart)) &&
      (oopisGreaterThanOrEqualTo(address, newSpaceStart))) {
    return (oopisGreaterThanOrEqualToandLessThan(address, ((eden).start),
                                                 freeStart)) ||
           ((oopisGreaterThanOrEqualToandLessThan(address, ((pastSpace).start),
                                                  pastSpaceStart)) ||
            ((gcPhaseInProgress == ScavengeInProgress) &&
             (oopisGreaterThanOrEqualToandLessThan(
                 address, ((futureSpace).start), futureSurvivorStart))));
  }
  addressUsqInt = address;

  /* begin isInSegments: */
  for (i = 0; i < numSegments; i += 1) {
    if (addressUsqInt < (((segments[i]).segStart))) {
      return 0;
    }
    if (addressUsqInt <
        ((((segments[i]).segSize)) + (((segments[i]).segStart)))) {
      return 1;
    }
  }
  return 0;
}