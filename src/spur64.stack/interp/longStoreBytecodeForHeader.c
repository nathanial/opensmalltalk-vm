/* Extracted from interp.c:56050 (function longStoreBytecodeForHeader). */

sqInt
longStoreBytecodeForHeader(sqInt methodHeader)
{
	return ((((sqLong) methodHeader)) < 0
			? AltLongStoreBytecode
			: LongStoreBytecode);
}