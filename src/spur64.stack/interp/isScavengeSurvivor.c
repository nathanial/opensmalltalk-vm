/* Extracted from interp.c:28426 (function isScavengeSurvivor). */

/*	Answer whether the oop has survived a scavenge. This is equivalent to
	| target |
	(manager isImmediate: oop) ifTrue:
	[^true].
	target := (manager isForwarded: oop)
	ifTrue: [manager followForwarded: oop]
	ifFalse: [oop].
	^((manager isInEden: target)
	or: [(manager isInPastSpace: target)]) not */

	/* SpurGenerationScavenger>>#isScavengeSurvivor: */

static NoDbgRegParms sqInt
isScavengeSurvivor(sqInt oop)
{
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
	oopisLessThan(target, oldSpaceStart)))))
	 || (oopisGreaterThanOrEqualToandLessThan(target, ((futureSpace).start), futureSurvivorStart));
}