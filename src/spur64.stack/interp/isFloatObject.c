/* Extracted from interp.c:54975 (function isFloatObject). */

int
isFloatObject(sqInt oop)
{
    sqInt tagBits;

	return /* isFloatInstance: */
		((tagBits = oop & (tagMask()))
			? tagBits == (smallFloatTag())
			: ((longAt((void *)(oop))) & (classIndexMask())) == ClassFloatCompactIndex);
}