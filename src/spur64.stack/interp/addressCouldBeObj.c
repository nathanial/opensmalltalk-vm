/* Extracted from interp.c:29737 (function addressCouldBeObj). */

sqInt
addressCouldBeObj(sqInt address)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return ((address & (BaseHeaderSize - 1)) == 0)
	 && ((oopisGreaterThanOrEqualTo(address, GIV(oldSpaceStart))
			? oopisLessThan(address, GIV(endOfMemory))
			: (oopisGreaterThanOrEqualToandLessThan(address, ((GIV(eden)).start), GIV(freeStart)))
			 || ((oopisGreaterThanOrEqualToandLessThan(address, ((GIV(pastSpace)).start), GIV(pastSpaceStart)))
			 || ((GIV(gcPhaseInProgress) == ScavengeInProgress)
			 && (oopisGreaterThanOrEqualToandLessThan(address, ((GIV(futureSpace)).start), GIV(futureSurvivorStart)))))));
}