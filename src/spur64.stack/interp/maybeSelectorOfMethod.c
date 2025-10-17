/* Extracted from interp.c:57656 (function maybeSelectorOfMethod). */

/*	Answer the selector of a method, assuming its penultimate literal is
	either a symbol or a pointer object whose first slot references the method
	and whose second slot is a symbol (i.e. an AdditionalMethodState). If a
	Symbol can't be found answer nil. This isn't satisfactory, as it puts a
	lot of information
	into the VM, but it is needed for adequate crash debugging at Cadence.
	With full blocks as of 9/2016 the last literal of a CompiledBlock is a
	back pointer
	to the enclosing block or compiled method. */

	/* StackInterpreter>>#maybeSelectorOfMethod: */

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
		return (((longAt((void *)(penultimateLiteral))) & (classIndexMask())) == (fetchClassTagOfNonImm(longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(SelectorDoesNotUnderstand) << (shiftForWord()))))))))
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
		 && (((longAt((void *)(maybeSelector))) & (classIndexMask())) == (fetchClassTagOfNonImm(longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(SelectorDoesNotUnderstand) << (shiftForWord())))))))))))))
			? maybeSelector
			: 0);
}