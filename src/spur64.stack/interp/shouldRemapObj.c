/* Extracted from interp.c:48065 (function shouldRemapObj). */

/*	Answer if the obj should be scavenged, or simply followed. Sent via the
	compactor from shouldRemapObj:. We test for being already scavenged
	because mapStackPages
	via mapInterpreterOops may be applied twice in the context of a global GC
	where a
	scavenge, followed by a scan-mark-free, and final compaction passes may
	result in
	scavenged fields being visited twice. */

	/* SpurPlanningCompactor>>#shouldRemapObj: */

sqInt
shouldRemapObj(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return /* slidingCompactionShouldRemapObj: */
		((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))
	 || ((gcPhaseInProgress > 0)
	 && ((gcPhaseInProgress == ScavengeInProgress
			? ((/* begin isReallyYoungObject: *//* begin isYoungObject: */
				assert(isNonImmediate(objOop)),
			oopisLessThan(objOop, oldSpaceStart)))
			 && (!(oopisGreaterThanOrEqualToandLessThan(objOop, ((futureSpace).start), futureSurvivorStart)))
			: /* isMobile: */
				(oopisGreaterThanOrEqualToandLessThanOrEqualTo(objOop, mobileStart, lastMobileObject))
			 && (!(((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) != 0)))));
}