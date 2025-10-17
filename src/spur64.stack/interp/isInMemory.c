/* Extracted from interp.c:37374 (function isInMemory). */

/*	Answer if the given address is in ST object memory. */

	/* SpurMemoryManager>>#isInMemory: */

sqInt
isInMemory(sqInt address)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt addressUsqInt;
    sqInt i;

	if (/* isInNewSpace: */
		(oopisLessThan(address, GIV(oldSpaceStart)))
	 && (oopisGreaterThanOrEqualTo(address, GIV(newSpaceStart)))) {
		return (oopisGreaterThanOrEqualToandLessThan(address, ((GIV(eden)).start), GIV(freeStart)))
		 || ((oopisGreaterThanOrEqualToandLessThan(address, ((GIV(pastSpace)).start), GIV(pastSpaceStart)))
		 || ((GIV(gcPhaseInProgress) == ScavengeInProgress)
		 && (oopisGreaterThanOrEqualToandLessThan(address, ((GIV(futureSpace)).start), GIV(futureSurvivorStart)))));
	}
	addressUsqInt = address;

	/* begin isInSegments: */
	for (i = 0; i < GIV(numSegments); i += 1) {
		if (addressUsqInt < (((GIV(segments)[i]).segStart))) {
			return 0;
		}
		if (addressUsqInt < ((((GIV(segments)[i]).segSize)) + (((GIV(segments)[i]).segStart)))) {
			return 1;
		}
	}
	return 0;
}