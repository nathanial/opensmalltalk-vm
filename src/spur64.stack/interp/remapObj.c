/* Extracted from interp.c:47945 (function remapObj). */

sqInt
remapObj(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt resolvedObj;

	/* begin slidingCompactionRemapObj: */
	assert(shouldRemapOop(objOop));
	if ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
		/* begin followForwarded: */
		assert(isUnambiguouslyForwarder(objOop));
		resolvedObj = longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
		while (/* isOopForwarded: */
			((!(resolvedObj & (tagMask()))))
		 && ((!((longAt((void *)(resolvedObj))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
			resolvedObj = longAt((void *)((resolvedObj + BaseHeaderSize) + (0U << (shiftForWord()))));
		}
	}
	else {
		assert(!((isInFutureSpace(objOop))));
		resolvedObj = objOop;
	}
	if (GIV(gcPhaseInProgress) > 0) {
		if (GIV(gcPhaseInProgress) == ScavengeInProgress) {
			if ((/* isReallyYoung: */
				((!(resolvedObj & (tagMask()))))
			 && ((/* begin isReallyYoungObject: *//* begin isYoungObject: */
				assert(isNonImmediate(resolvedObj)),
			oopisLessThan(resolvedObj, GIV(oldSpaceStart)))))
			 && (!(oopisGreaterThanOrEqualToandLessThan(resolvedObj, ((GIV(futureSpace)).start), GIV(futureSurvivorStart))))) {
				return copyAndForward(resolvedObj);
			}
		}
		else {
			assert(slidingCompactionInProgress());
			if (/* isMobile: */
				(oopisGreaterThanOrEqualToandLessThanOrEqualTo(objOop, GIV(mobileStart), GIV(lastMobileObject)))
			 && (!(((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) != 0))) {
				return longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
			}
		}
	}
	return resolvedObj;
}