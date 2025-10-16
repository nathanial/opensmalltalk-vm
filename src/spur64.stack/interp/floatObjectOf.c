/* Extracted from interp.c:26929 (function floatObjectOf). */

sqInt
floatObjectOf(double aFloat)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt newFloatObj;
    usqInt newObj;
    usqInt numBytes;
    sqInt numSlots;
    usqLong rawFloat;
    usqLong rot;

	if (isSmallFloatValue(aFloat)) {
		/* begin smallFloatObjectOf: */
		assert(isSmallFloatValue(aFloat));
		memcpy((&rawFloat), (&aFloat), sizeof(rawFloat));

		/* begin smallFloatObjectOfBits: */
		assert(isSmallFloatValueBits(rawFloat));
		rot = (((rawFloat) >> 0x3F) & 1) + ((rawFloat << 1));
		if (rot > 1) {
			rot -= (((usqInt)((smallFloatExponentOffset())) << ((smallFloatMantissaBits()) + 1)));
			assert(rot > 0);
		}

		/* a.k.a. ~= +/-0.0 */
		return ((rot << (numTagBits()))) + (smallFloatTag());
	}
	numSlots = (sizeof(double)) / BytesPerOop;

	/* begin eeInstantiateSmallClassIndex:format:numSlots: */
	assert((numSlots >= 0)
	 && ((knownClassAtIndex(ClassFloatCompactIndex)) != GIV(nilObj)));
	assert((firstLongFormat()) == (instSpecOfClass(knownClassAtIndex(ClassFloatCompactIndex))));

	/* begin allocateSmallNewSpaceSlots:format:classIndex: */
	assert(numSlots < (numSlotsMask()));
	newObj = GIV(freeStart);
	numBytes = BaseHeaderSize + ((numSlots < 1
		? 8 /* allocationUnit */
		: numSlots * BytesPerOop));
	assert((numBytes % (allocationUnit())) == 0);
	assert((newObj % (allocationUnit())) == 0);
	if ((GIV(freeStart) + numBytes) > GIV(scavengeThreshold)) {
		if (!GIV(needGCFlag)) {
			/* begin scheduleScavenge */
			GIV(needGCFlag) = 1;
			forceInterruptCheck();
		}
		if ((GIV(freeStart) + numBytes) > (((GIV(eden)).limit))) {
			error("no room in eden for allocateSmallNewSpaceSlots:format:classIndex:");
			newFloatObj = 0;
			goto l1;
		}
	}
	long64Atput((void *)(newObj),((((((usqLong) numSlots)) << (numSlotsFullShift()))) + ((((usqInt)((firstLongFormat())) << (formatShift()))))) + ClassFloatCompactIndex);
	GIV(freeStart) += numBytes;
	newFloatObj = newObj;
	/* end eeInstantiateSmallClassIndex:format:numSlots: */
l1:
	storeFloatAtfrom(newFloatObj + BaseHeaderSize, aFloat);
	return newFloatObj;
}