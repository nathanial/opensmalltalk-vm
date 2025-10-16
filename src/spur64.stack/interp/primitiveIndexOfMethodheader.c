/* Extracted from interp.c:59030 (function primitiveIndexOfMethodheader). */

sqInt
primitiveIndexOfMethodheader(sqInt theMethod, sqInt methodHeader)
{
    usqInt firstBytecode;

	return (((methodHeader & AlternateHeaderHasPrimFlag) != 0)
			? ((firstBytecode = (theMethod + ((LiteralStart + (((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask)) * BytesPerOop)) + BaseHeaderSize),
			(byteAt((void *)(firstBytecode + 1))) + ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8))))
			: 0);
}