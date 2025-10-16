/* Extracted from interp.c:25979 (function signed64BitValueOf). */

sqLong
signed64BitValueOf(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt ccIndex;
    sqInt fmt;
    usqLong magnitude;
    sqInt negative;
    usqInt numSlots;
    int ok;
    sqInt sz;

	if ((((oop) & 7) == 1)) {
		return ((sqLong) ((oop >> 3)));
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
	if (ok) {
		negative = 0;
	}
	else {
		negative = 1;

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
	if (sz > 4) {
		magnitude = SQ_SWAP_8_BYTES_IF_BIGENDIAN((long64At((void *)((oop + BaseHeaderSize)))));
		if ((sz > (sizeof(sqLong)))
		 || ((negative
				? magnitude > 0x8000000000000000ULL
				: magnitude >= 0x8000000000000000ULL))) {
			/* begin primitiveFail */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
			return 0;
		}
	}
	else {
		magnitude = ((unsigned int) (SQ_SWAP_4_BYTES_IF_BIGENDIAN((long32At((void *)((oop + BaseHeaderSize)))))));
	}
	return (negative
			? 0 - magnitude
			: magnitude);
}