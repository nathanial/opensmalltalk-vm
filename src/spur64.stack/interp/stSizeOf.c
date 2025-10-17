/* Extracted from interp.c:64934 (function stSizeOf). */

/*	Answer the number of indexable fields in the given object. (i.e., what
	Smalltalk would return for <obj> size).
 */
/*	Note: Assume oop is not an immediate! */

	/* StackInterpreter>>#stSizeOf: */

sqInt
stSizeOf(sqInt oop)
{
    sqInt class;
    sqInt fixedFields;
    usqLong fmt;
    sqLong hdr;
    usqInt numSlots;
    usqInt numSlotsUsqInt;
    sqInt totalLength;

	hdr = long64At((void *)(oop));
	fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask());

	/* begin lengthOf:baseHeader:format: */
	/* begin lengthOf:format: */
	/* begin numSlotsOfAny: */
	numSlotsUsqInt = byteAt((void *)(oop + (numSlotsFieldByteOffset())));
	numSlots = (numSlotsUsqInt == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(oop - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlotsUsqInt);
	if (fmt <= (ephemeronFormat())) {
		totalLength = numSlots;
		goto l1;
	}
	if (fmt >= (firstByteFormat())) {
		totalLength = ((numSlots << (shiftForWord()))) - (fmt & 7);
		goto l1;
	}

	/* bytes, including CompiledMethod */
	if (fmt >= (firstShortFormat())) {
		totalLength = ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
		goto l1;
	}
	if (fmt >= (firstLongFormat())) {
		totalLength = ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
		goto l1;
	}
	if (fmt == (sixtyFourBitIndexableFormat())) {
		totalLength = numSlots;
		goto l1;
	}

	/* fmt = self forwardedFormat */
	totalLength = 0;
	/* end lengthOf:baseHeader:format: */
l1:
	if (/* isPureBitsFormat: */
		(fmt >= (sixtyFourBitIndexableFormat()))
	 && (fmt < (firstCompiledMethodFormat()))) {
		return totalLength;
	}

	/* begin fixedFieldsOf:format:length: */
	if ((fmt >= (sixtyFourBitIndexableFormat()))
	 || (fmt == (arrayFormat()))) {
		fixedFields = 0;
		goto l2;
	}
	if (fmt < (arrayFormat())) {
		fixedFields = totalLength;
		goto l2;
	}
	class = fetchClassOfNonImm(oop);
	fixedFields = (((longAt((void *)((class + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3)) & ((1U << (fixedFieldsFieldWidth())) - 1);
	/* end fixedFieldsOf:format:length: */
l2:
	if (fmt == (indexablePointersFormat())) {
		assert(!(isContextHeader(hdr)));
	}
	return totalLength - fixedFields;
}