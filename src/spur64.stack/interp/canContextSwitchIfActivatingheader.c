/* Extracted from interp.c:50356 (function canContextSwitchIfActivatingheader). */

sqInt
canContextSwitchIfActivatingheader(sqInt theMethod, sqInt methodHeader)
{
    usqInt firstBytecode;
    sqInt primitiveIndex;

	/* primitiveIndexOfMethod:header: */
	if (((methodHeader & AlternateHeaderHasPrimFlag) != 0)) {
		firstBytecode = (theMethod + ((LiteralStart + (((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask)) * BytesPerOop)) + BaseHeaderSize;
		primitiveIndex = (byteAt((void *)(firstBytecode + 1))) + ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8)));
	}
	else {
		primitiveIndex = 0;
	}
	return (primitiveIndex != PrimNumberUnwindMarker)
			 && (primitiveIndex != PrimNumberNoContextSwitchMarker);
}