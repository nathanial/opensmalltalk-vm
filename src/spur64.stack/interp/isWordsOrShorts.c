/* Extracted from interp.c:37890 (function isWordsOrShorts). */

/*	Answer if the argument contains only indexable 16-bit half words or 32-bit
	indexable words (no oops).
	See comment in formatOf: */

	/* SpurMemoryManager>>#isWordsOrShorts: */

sqInt
isWordsOrShorts(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (((((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) >= (firstLongFormat())) && (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) <= ((firstByteFormat()) - 1))));
}