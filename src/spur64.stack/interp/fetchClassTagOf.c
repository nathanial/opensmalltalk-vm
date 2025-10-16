/* Extracted from interp.c:26918 (function fetchClassTagOf). */

sqInt
fetchClassTagOf(sqInt oop)
{
    sqInt tagBits;

	return ((tagBits = oop & (tagMask()))
			? tagBits
			: (longAt((void *)(oop))) & (classIndexMask()));
}