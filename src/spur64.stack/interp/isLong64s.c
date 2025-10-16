/* Extracted from interp.c:37437 (function isLong64s). */

sqInt
isLong64s(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) == (sixtyFourBitIndexableFormat()));
}