/* Extracted from interp.c:62709 (function safeMethodClassOf). */

/*	Safe version of methodClassOf: that deals with malformed compiled methods,
	etc, and does not fixup forwarding pointers.. */

	/* StackInterpreter>>#safeMethodClassOf: */

static NoDbgRegParms sqInt
safeMethodClassOf(sqInt methodPointer)
{
    sqInt literal;
    sqInt maybeClass;
    usqInt numSlots;
    sqInt offset;

	offset = (literalCountOf(methodPointer)) - 1;

	/* begin literal:ofMethod: */
	literal = longAt((void *)((methodPointer + BaseHeaderSize) + ((((usqInt)((offset + LiteralStart)) << (shiftForWord()))))));
	if (/* isOopForwarded: */
		((!(literal & (tagMask()))))
	 && ((!((longAt((void *)(literal))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		literal = followForwarded(literal);
	}
	if (!((/* isPointers: */
			((!(literal & (tagMask()))))
		 && (((byteAt((void *)(literal + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */))
		 && (((/* begin numSlotsOf: */
			assert((classIndexOf(literal)) > (isForwardedObjectClassIndexPun())),
		(((numSlots = byteAt((void *)(literal + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(literal - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlots))) > ValueIndex))) {
		return nilObj;
	}
	maybeClass = longAt((void *)((literal + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));
	if (/* isOopForwarded: */
		((!(maybeClass & (tagMask()))))
	 && ((!((longAt((void *)(maybeClass))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		maybeClass = followForwarded(maybeClass);
	}
	return maybeClass;
}