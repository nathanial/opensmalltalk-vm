/* Extracted from interp.c:35386 (function firstIndexableField). */

void *
firstIndexableField(sqInt objOop)
{
    sqInt classFormat;
    sqInt classPointer;
    sqInt fmt;

	fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
	if (fmt <= (weakArrayFormat())) {
		if (fmt == (arrayFormat())) {
			return pointerForOop(objOop + BaseHeaderSize);
		}

		/* array starts at 0. */
		if (fmt >= (indexablePointersFormat())) {
			classPointer = fetchClassOfNonImm(objOop);

			/* begin formatOfClass: */
			classFormat = ((longAt((void *)((classPointer + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3);
			return pointerForOop((objOop + BaseHeaderSize) + ((((usqInt)((classFormat & ((1U << (fixedFieldsFieldWidth())) - 1))) << (shiftForWord())))));
		}

		/* indexable with inst vars; need to delve into the class format word
		   otherwise not indexable */
		return 0;
	}

	/* All bit objects, and indeed CompiledMethod, though this is a no-no, start at 0 */
	if ((fmt >= (sixtyFourBitIndexableFormat()))
	 && (fmt < (firstCompiledMethodFormat()))) {
		return pointerForOop(objOop + BaseHeaderSize);
	}

	/* otherwise not indexable */
	return 0;
}