/* Extracted from interp.c:57726 (function methodClassOf). */

/*	Answer the method class of a method which is the value of an Association
	in the last literal,
	or answer nil if there isn't one.
	Using a read barrier here simplifies the become implementation and costs
	very little
	because the class index and ValueIndex of the association almost certainly
	share a cache line. */

	/* StackInterpreter>>#methodClassOf: */

sqInt
methodClassOf(sqInt methodPointer)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt literal;
    sqInt offset;

	offset = (literalCountOf(methodPointer)) - 1;

	/* begin followLiteral:ofMethod: */
	/* begin followField:ofObject: */
	literal = longAt((void *)((methodPointer + BaseHeaderSize) + ((((usqInt)((offset + LiteralStart)) << (shiftForWord()))))));
	if (/* isOopForwarded: */
		((!(literal & (tagMask()))))
	 && ((!((longAt((void *)(literal))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		literal = fixFollowedFieldofObjectwithInitialValue(offset + LiteralStart, methodPointer, literal);
	}
	return ((literal != GIV(nilObj))
	 && (/* isPointers: */
		((!(literal & (tagMask()))))
	 && (((byteAt((void *)(literal + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */))
			? (assert((numSlotsOf(literal)) > ValueIndex),
			followFieldofObject(ValueIndex, literal))
			: GIV(nilObj));
}