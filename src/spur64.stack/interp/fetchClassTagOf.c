/* Extracted from interp.c:26918 (function fetchClassTagOf). */

/*	Answer the tag used in lookup caches for a receiver. This is the
	receiver's classIndex. */

	/* Spur64BitMemoryManager>>#fetchClassTagOf: */

sqInt
fetchClassTagOf(sqInt oop)
{
    sqInt tagBits;

	return ((tagBits = oop & (tagMask()))
			? tagBits
			: (longAt((void *)(oop))) & (classIndexMask()));
}