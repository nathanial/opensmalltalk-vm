/* Extracted from interp.c:28426 (function isScavengeSurvivor). */

static NoDbgRegParms sqInt
isScavengeSurvivor(sqInt oop)
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
	}
	else {
		target = oop;
	}
	return (!(/* isReallyYoung: */
		((!(target & (tagMask()))))
	 && ((/* begin isReallyYoungObject: *//* begin isYoungObject: */
		assert(isNonImmediate(target)),
	oopisLessThan(target, GIV(oldSpaceStart))))))
	 || (oopisGreaterThanOrEqualToandLessThan(target, ((GIV(futureSpace)).start), GIV(futureSurvivorStart)));
}