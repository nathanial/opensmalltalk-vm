/* Extracted from interp.c:28384 (function isMaybeOldScavengeSurvivor). */

static NoDbgRegParms sqInt
isMaybeOldScavengeSurvivor(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt target;

	if (((oop & (tagMask())) != 0)) {
		return 1;
	}
	if ((!((longAt((void *)(oop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
		/* begin followForwarded: */
		assert(isUnambiguouslyForwarder(oop));
		target = longAt((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))));
		while (/* isOopForwarded: */
			((!(target & (tagMask()))))
		 && ((!((longAt((void *)(target))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
			target = longAt((void *)((target + BaseHeaderSize) + (0U << (shiftForWord()))));
		}
		if (((oop & (tagMask())) != 0)) {
			return 1;
		}
	}
	else {
		target = oop;
	}
	return (oopisGreaterThanOrEqualTo(target, GIV(oldSpaceStart))
			? (GIV(tenureCriterion) != MarkOnTenure)
			 || (((byteAt((void *)(target + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0)
			: oopisGreaterThanOrEqualToandLessThan(target, ((GIV(futureSpace)).start), GIV(futureSurvivorStart)));
}