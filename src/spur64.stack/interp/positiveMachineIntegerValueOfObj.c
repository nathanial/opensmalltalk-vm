/* Extracted from interp.c:11525 (function positiveMachineIntegerValueOfObj). */

/*	Answer a value of an integer in address range, i.e up to the size of a
	machine word.
	The object is known not to be a SmallIntege. It is hoped to be a
	LargePositiveInteger of size <= word size.
 */

	/* InterpreterPrimitives>>#positiveMachineIntegerValueOfObj: */

static NoDbgRegParms usqIntptr_t
positiveMachineIntegerValueOfObj(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt bs;
    sqInt ccIndex;
    sqInt fmt;
    sqInt isClassOfNonImmequalTocompactClassIndexRV;
    usqInt numSlots;

	assert(!(((((oop) & 7) == 1))));
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
	isClassOfNonImmequalTocompactClassIndexRV = ClassLargePositiveIntegerCompactIndex == ccIndex;
	if (!isClassOfNonImmequalTocompactClassIndexRV) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return 0;
	}

	/* begin numBytesOfBytes: */
	fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
	assert(fmt >= (firstByteFormat()));
	bs = ((((/* begin numSlotsOf: */
	assert((classIndexOf(oop)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(oop + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(oop - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) << (shiftForWord()))) - (fmt & 7);
	if (bs > (sizeof(usqIntptr_t))) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return 0;
	}
	if (((sizeof(usqIntptr_t)) == 8)
	 && (bs > 4)) {
		return SQ_SWAP_8_BYTES_IF_BIGENDIAN((long64At((void *)((oop + BaseHeaderSize)))));
	}
	return ((unsigned int) (SQ_SWAP_4_BYTES_IF_BIGENDIAN((long32At((void *)((oop + BaseHeaderSize)))))));
}