/* Extracted from interp.c:57752 (function methodPrimitiveIndex). */

	/* StackInterpreter>>#methodPrimitiveIndex */

sqInt
methodPrimitiveIndex(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt firstBytecode;
    sqInt methodHeader;

	if (!((addressCouldBeObj(newMethod))
		 && (((byteAt((void *)(newMethod + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))) {
		return -1;
	}

	/* begin primitiveIndexOf: */
	/* begin methodHeaderOf: */
	assert(isCompiledMethod(newMethod));
	methodHeader = longAt((void *)((newMethod + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

	/* begin primitiveIndexOfMethod:header: */
	return (((methodHeader & AlternateHeaderHasPrimFlag) != 0)
			? ((firstBytecode = (newMethod + ((LiteralStart + (((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask)) * BytesPerOop)) + BaseHeaderSize),
			(byteAt((void *)(firstBytecode + 1))) + ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8))))
			: 0);
}