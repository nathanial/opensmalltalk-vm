/* Extracted from interp.c:57656 (function maybeSelectorOfMethod). */

sqInt
maybeSelectorOfMethod(sqInt methodObj)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt maybeSelector;
    usqInt numSlots;
    sqInt penultimateLiteral;
    sqInt ultimateLiteral;

	assert(isOopCompiledMethod(methodObj));
	ultimateLiteral = ultimateLiteralOf(methodObj);
	if (/* isOopCompiledMethod: */
		((!(ultimateLiteral & (tagMask()))))
	 && (((byteAt((void *)(ultimateLiteral + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat()))) {
		return maybeSelectorOfMethod(ultimateLiteral);
	}
	penultimateLiteral = penultimateLiteralOf(methodObj);
	if (/* isWordsOrBytes: */
		((!(penultimateLiteral & (tagMask()))))
	 && (isWordsOrBytesNonImm(penultimateLiteral))) {
		return (((longAt((void *)(penultimateLiteral))) & (classIndexMask())) == (fetchClassTagOfNonImm(longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SelectorDoesNotUnderstand) << (shiftForWord()))))))))
				? penultimateLiteral
				: 0);
	}
	return ((/* isPointers: */
		((!(penultimateLiteral & (tagMask()))))
	 && (((byteAt((void *)(penultimateLiteral + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */))
	 && ((((/* begin numSlotsOf: */
		assert((classIndexOf(penultimateLiteral)) > (isForwardedObjectClassIndexPun())),
	(((numSlots = byteAt((void *)(penultimateLiteral + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(penultimateLiteral - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots))) >= 2)
	 && (((longAt((void *)((penultimateLiteral + BaseHeaderSize) + (0U << (shiftForWord()))))) == methodObj)
	 && (((maybeSelector = longAt((void *)((penultimateLiteral + BaseHeaderSize) + (1U << (shiftForWord()))))),
	(/* isWordsOrBytes: */
			((!(maybeSelector & (tagMask()))))
		 && (isWordsOrBytesNonImm(maybeSelector)))
		 && (((longAt((void *)(maybeSelector))) & (classIndexMask())) == (fetchClassTagOfNonImm(longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SelectorDoesNotUnderstand) << (shiftForWord())))))))))))))
			? maybeSelector
			: 0);
}