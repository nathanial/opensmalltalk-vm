/* Extracted from interp.c:11352 (function magnitude64BitValueOf). */

static NoDbgRegParms usqLong
magnitude64BitValueOf(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt ccIndex;
    sqInt fmt;
    usqInt numSlots;
    int ok;
    sqInt smallIntValue;
    sqInt sz;

	if ((((oop) & 7) == 1)) {
		smallIntValue = (oop >> 3);
		if (smallIntValue < 0) {
			smallIntValue = 0 - smallIntValue;
		}
		return smallIntValue;
	}
	if (((oop & (tagMask())) != 0)) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return 0;
	}

	/* begin isClassOfNonImm:equalTo:compactClassIndex: */
	assert(!(isImmediate(oop)));
	ccIndex = (longAt((void *)(oop))) & (classIndexMask());
	ok = ClassLargePositiveIntegerCompactIndex == ccIndex;
	if (!ok) {
		/* begin isClassOfNonImm:equalTo:compactClassIndex: */
		assert(!(isImmediate(oop)));
		ccIndex = (longAt((void *)(oop))) & (classIndexMask());
		ok = ClassLargeNegativeIntegerCompactIndex == ccIndex;
		if (!ok) {
			/* begin primitiveFail */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
			return 0;
		}
	}

	/* begin numBytesOfBytes: */
	fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
	assert(fmt >= (firstByteFormat()));
	sz = ((((/* begin numSlotsOf: */
	assert((classIndexOf(oop)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(oop + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(oop - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) << (shiftForWord()))) - (fmt & 7);
	if (sz > (sizeof(sqLong))) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return 0;
	}
	if (sz > 4) {
		return SQ_SWAP_8_BYTES_IF_BIGENDIAN((long64At((void *)((oop + BaseHeaderSize)))));
	}
	return ((unsigned int) (SQ_SWAP_4_BYTES_IF_BIGENDIAN((long32At((void *)((oop + BaseHeaderSize)))))));
}