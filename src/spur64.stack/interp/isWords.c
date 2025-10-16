/* Extracted from interp.c:37903 (function isWords). */

sqInt
isWords(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (((((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) >= (firstLongFormat())) && (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) <= ((firstShortFormat()) - 1))));
}