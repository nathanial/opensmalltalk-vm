/* Extracted from interp.c:37890 (function isWordsOrShorts). */

sqInt
isWordsOrShorts(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (((((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) >= (firstLongFormat())) && (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) <= ((firstByteFormat()) - 1))));
}