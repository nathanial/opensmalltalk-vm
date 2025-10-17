/* Extracted from interp.c:26929 (function floatObjectOf). */

	/* Spur64BitMemoryManager>>#floatObjectOf: */

sqInt
floatObjectOf(double aFloat)
{
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
	 && ((knownClassAtIndex(ClassFloatCompactIndex)) != nilObj));
	assert((firstLongFormat()) == (instSpecOfClass(knownClassAtIndex(ClassFloatCompactIndex))));

	/* begin allocateSmallNewSpaceSlots:format:classIndex: */
	assert(numSlots < (numSlotsMask()));
	newObj = freeStart;
	numBytes = BaseHeaderSize + ((numSlots < 1
		? 8 /* allocationUnit */
		: numSlots * BytesPerOop));
	assert((numBytes % (allocationUnit())) == 0);
	assert((newObj % (allocationUnit())) == 0);
	if ((freeStart + numBytes) > scavengeThreshold) {
		if (!needGCFlag) {
			/* begin scheduleScavenge */
			needGCFlag = 1;
			forceInterruptCheck();
		}
		if ((freeStart + numBytes) > (((eden).limit))) {
			error("no room in eden for allocateSmallNewSpaceSlots:format:classIndex:");
			newFloatObj = 0;
			goto l1;
		}
	}
	long64Atput((void *)(newObj),((((((usqLong) numSlots)) << (numSlotsFullShift()))) + ((((usqInt)((firstLongFormat())) << (formatShift()))))) + ClassFloatCompactIndex);
	freeStart += numBytes;
	newFloatObj = newObj;
	/* end eeInstantiateSmallClassIndex:format:numSlots: */
l1:
	storeFloatAtfrom(newFloatObj + BaseHeaderSize, aFloat);
	return newFloatObj;
}