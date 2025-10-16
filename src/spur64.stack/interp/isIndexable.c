/* Extracted from interp.c:37344 (function isIndexable). */

sqInt
isIndexable(sqInt objOop)
{
    sqInt fmt;

	fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
	return /* isIndexableFormat: */
		(fmt >= (arrayFormat()))
	 && ((fmt <= (weakArrayFormat()))
	 || (fmt >= (sixtyFourBitIndexableFormat())));
}