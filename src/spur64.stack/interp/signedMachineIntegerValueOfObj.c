/* Extracted from interp.c:26060 (function signedMachineIntegerValueOfObj). */

/*	Answer a signed value of an integer up to the size of a machine word.
	The object is not an immediate and is hoped to be a LargeInteger of size
	<= word size.
 */

	/* InterpreterPrimitives>>#signedMachineIntegerValueOfObj: */

static NoDbgRegParms sqIntptr_t
signedMachineIntegerValueOfObj(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt bs;
    sqInt ccIndex;
    sqInt fmt;
    sqInt isClassOfNonImmequalTocompactClassIndexRV;
    usqIntptr_t limit;
    usqIntptr_t magnitude;
    sqInt negative;
    usqInt numBytes;
    usqInt numSlots;
    usqIntptr_t value;

	assert(!(((((oop) & 7) == 1))));
	if (((oop & (tagMask())) != 0)) {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		return null;
	}

	/* begin isClassOfNonImm:equalTo:compactClassIndex: */
	assert(!(isImmediate(oop)));
	ccIndex = (longAt((void *)(oop))) & (classIndexMask());
	isClassOfNonImmequalTocompactClassIndexRV = ClassLargePositiveIntegerCompactIndex == ccIndex;
	if (isClassOfNonImmequalTocompactClassIndexRV) {
		negative = 0;
	}
	else {
		negative = 1;

		/* begin isClassOfNonImm:equalTo:compactClassIndex: */
		assert(!(isImmediate(oop)));
		ccIndex = (longAt((void *)(oop))) & (classIndexMask());
		isClassOfNonImmequalTocompactClassIndexRV = ClassLargeNegativeIntegerCompactIndex == ccIndex;
		if (!isClassOfNonImmequalTocompactClassIndexRV) {
			/* begin primitiveFail */
			if (!primFailCode) {
				primFailCode = 1;
			}
			return null;
		}
	}

	/* begin numBytesOf: */
	fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
	assert((classIndexOf(oop)) > (isForwardedObjectClassIndexPun()));
	numBytes = (((numSlots = byteAt((void *)(oop + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(oop - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots);
	numBytes = (numBytes << (shiftForWord()));
	if (fmt >= (firstByteFormat())) {
		bs = numBytes - (fmt & 7);
		goto l1;
	}

	/* bytes (the common case), including CompiledMethod */
	if (fmt <= (sixtyFourBitIndexableFormat())) {
		bs = numBytes;
		goto l1;
	}
	if (fmt >= (firstShortFormat())) {
		bs = numBytes - (((fmt & 3) << 1));
		goto l1;
	}

	/* fmt >= self firstLongFormat */
	bs = numBytes - (((fmt & 1) << 2));
	/* end numBytesOf: */
l1:
	if (bs > (sizeof(usqIntptr_t))) {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		return null;
	}
	if (((sizeof(sqIntptr_t)) == 8)
	 && (bs > 4)) {
		magnitude = SQ_SWAP_8_BYTES_IF_BIGENDIAN((long64At((void *)((oop + BaseHeaderSize)))));
	}
	else {
		magnitude = ((unsigned int) (SQ_SWAP_4_BYTES_IF_BIGENDIAN((long32At((void *)((oop + BaseHeaderSize)))))));
	}
	limit = ((((usqIntptr_t)1)) << (((sizeof(sqIntptr_t)) * 8) - 1));
	if ((negative
			? magnitude > limit
			: magnitude >= limit)) {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		return 0;
	}
	if (negative) {
		value = 0 - magnitude;
	}
	else {
		value = magnitude;
	}
	return value;
}