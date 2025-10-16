/* Extracted from interp.c:48065 (function shouldRemapObj). */

sqInt
shouldRemapObj(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return /* slidingCompactionShouldRemapObj: */
		((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))
	 || ((GIV(gcPhaseInProgress) > 0)
	 && ((GIV(gcPhaseInProgress) == ScavengeInProgress
			? ((/* begin isReallyYoungObject: *//* begin isYoungObject: */
				assert(isNonImmediate(objOop)),
			oopisLessThan(objOop, GIV(oldSpaceStart))))
			 && (!(oopisGreaterThanOrEqualToandLessThan(objOop, ((GIV(futureSpace)).start), GIV(futureSurvivorStart))))
			: /* isMobile: */
				(oopisGreaterThanOrEqualToandLessThanOrEqualTo(objOop, GIV(mobileStart), GIV(lastMobileObject)))
			 && (!(((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) != 0)))));
}