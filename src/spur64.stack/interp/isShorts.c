/* Extracted from interp.c:37627 (function isShorts). */

sqInt
isShorts(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (((((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) >= (firstShortFormat())) && (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) <= ((firstByteFormat()) - 1))));
}