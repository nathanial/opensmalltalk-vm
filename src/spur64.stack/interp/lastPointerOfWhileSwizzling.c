/* Extracted from interp.c:38025 (function lastPointerOfWhileSwizzling). */

static NoDbgRegParms sqInt
lastPointerOfWhileSwizzling(sqInt objOop)
{
    sqInt fmt;
    sqInt header;
    usqInt numSlots;

	fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
	assert(fmt != (forwardedFormat()));
	if (fmt <= 5 /* lastPointerFormat */) {
		return ((((/* begin numSlotsOf: */
	assert((classIndexOf(objOop)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) - 1) * BytesPerOop) + BaseHeaderSize;
	}
	if (fmt < (firstCompiledMethodFormat())) {
		return 0;
	}

	/* begin methodHeaderOf: */
	assert(isCompiledMethod(objOop));
	header = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
	return (((((/* begin literalCountOfMethodHeader: */
	assert((((header) & 7) == 1)),
/* literalCountOfAlternateHeader: */
	((header >> 3)) & AlternateHeaderNumLiteralsMask)) + LiteralStart) - 1) * BytesPerOop) + BaseHeaderSize;
}