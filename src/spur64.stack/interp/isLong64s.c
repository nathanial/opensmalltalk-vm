/* Extracted from interp.c:37437 (function isLong64s). */

/*	Answer if the argument contains only indexable 64-bit double words (no
	oops). See comment in formatOf:
 */

	/* SpurMemoryManager>>#isLong64s: */

sqInt
isLong64s(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) == (sixtyFourBitIndexableFormat()));
}