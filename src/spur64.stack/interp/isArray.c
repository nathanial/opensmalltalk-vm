/* Extracted from interp.c:37130 (function isArray). */

sqInt
isArray(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) == (arrayFormat()));
}