/* Extracted from interp.c:37160 (function isBytes). */

sqInt
isBytes(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat()));
}