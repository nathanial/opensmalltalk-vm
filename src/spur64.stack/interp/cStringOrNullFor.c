/* Extracted from interp.c:11049 (function cStringOrNullFor). */

/*	Answer either a malloced string with the null-terminated contents of oop
	if oop is a string,
	or the null pointer if oop is nil, or fail. It is the client's
	responsibility to free the string later. */

	/* InterpreterPrimitives>>#cStringOrNullFor: */

char *
cStringOrNullFor(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt ccIndex;
    char *cString;
    sqInt fmt;
    sqInt isString;
    sqInt len;
    usqInt numSlots;
    usqInt numSlotsUsqInt;

	/* begin isInstanceOfClassByteString: */
	/* begin is:instanceOf:compactClassIndex: */
	if (((oop & (tagMask())) != 0)) {
		isString = 0;
		goto l2;
	}

	/* begin isClassOfNonImm:equalTo:compactClassIndex: */
	assert(!(isImmediate(oop)));
	ccIndex = (longAt((void *)(oop))) & (classIndexMask());
	isString = ClassByteStringCompactIndex == ccIndex;
	/* end isInstanceOfClassByteString: */
l2:
	if (!isString) {
		if (oop != nilObj) {
			/* primitiveFailFor: */
			primFailCode = PrimErrBadArgument;
		}
		return 0;
	}

	/* begin lengthOf: */
	fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
	numSlotsUsqInt = byteAt((void *)(oop + (numSlotsFieldByteOffset())));
	numSlots = (numSlotsUsqInt == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(oop - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlotsUsqInt);
	if (fmt <= (ephemeronFormat())) {
		len = numSlots;
		goto l1;
	}
	if (fmt >= (firstByteFormat())) {
		len = ((numSlots << (shiftForWord()))) - (fmt & 7);
		goto l1;
	}

	/* bytes, including CompiledMethod */
	if (fmt >= (firstShortFormat())) {
		len = ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
		goto l1;
	}
	if (fmt >= (firstLongFormat())) {
		len = ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
		goto l1;
	}
	if (fmt == (sixtyFourBitIndexableFormat())) {
		len = numSlots;
		goto l1;
	}

	/* fmt = self forwardedFormat */
	len = 0;
	/* end lengthOf: */
l1:
	if (!len) {
		return 0;
	}
	cString = malloc(len + 1);
	if (!cString) {
		/* primitiveFailFor: */
		primFailCode = PrimErrNoCMemory;
		return 0;
	}
	memcpy(cString, firstIndexableField(oop), len);
	cString[len] = 0;
	return cString;
}