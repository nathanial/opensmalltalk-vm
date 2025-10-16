/* Extracted from interp.c:40697 (function maybeMethodClassOfseemsToBeInstantiating). */

sqInt
maybeMethodClassOfseemsToBeInstantiating(sqInt methodObj, sqInt format)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt instSpec;
    sqInt literal;
    sqInt maybeClassObj;
    sqInt maybeFormat;
    sqInt offset;

	/* begin methodClassOf: */
	offset = (literalCountOf(methodObj)) - 1;
	literal = longAt((void *)((methodObj + BaseHeaderSize) + ((((usqInt)((offset + LiteralStart)) << (shiftForWord()))))));
	if (/* isOopForwarded: */
		((!(literal & (tagMask()))))
	 && ((!((longAt((void *)(literal))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		literal = fixFollowedFieldofObjectwithInitialValue(offset + LiteralStart, methodObj, literal);
	}
	maybeClassObj = ((literal != GIV(nilObj))
		 && (/* isPointers: */
			((!(literal & (tagMask()))))
		 && (((byteAt((void *)(literal + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */))
				? (assert((numSlotsOf(literal)) > ValueIndex),
				followFieldofObject(ValueIndex, literal))
				: GIV(nilObj));
	if (!(((byteAt((void *)(maybeClassObj + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */)) {
		return 0;
	}
	if (!((numSlotsOfAny(maybeClassObj)) > InstanceSpecificationIndex)) {
		return 0;
	}
	maybeFormat = longAt((void *)((maybeClassObj + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))));
	if (!((((maybeFormat) & 7) == 1))) {
		return 0;
	}
	instSpec = (((usqInt)(((maybeFormat >> 3)))) >> (fixedFieldsFieldWidth())) & (formatMask());
	return instSpec == format;
}