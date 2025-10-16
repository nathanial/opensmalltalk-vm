/* Extracted from interp.c:57752 (function methodPrimitiveIndex). */

sqInt
methodPrimitiveIndex(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt firstBytecode;
    sqInt methodHeader;

	if (!((addressCouldBeObj(GIV(newMethod)))
		 && (((byteAt((void *)(GIV(newMethod) + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))) {
		return -1;
	}

	/* begin primitiveIndexOf: */
	/* begin methodHeaderOf: */
	assert(isCompiledMethod(GIV(newMethod)));
	methodHeader = longAt((void *)((GIV(newMethod) + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

	/* begin primitiveIndexOfMethod:header: */
	return (((methodHeader & AlternateHeaderHasPrimFlag) != 0)
			? ((firstBytecode = (GIV(newMethod) + ((LiteralStart + (((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask)) * BytesPerOop)) + BaseHeaderSize),
			(byteAt((void *)(firstBytecode + 1))) + ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8))))
			: 0);
}