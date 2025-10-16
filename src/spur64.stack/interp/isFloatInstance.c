/* Extracted from interp.c:27322 (function isFloatInstance). */

static NoDbgRegParms int
isFloatInstance(sqInt oop)
{
    sqInt tagBits;

	return ((tagBits = oop & (tagMask()))
			? tagBits == (smallFloatTag())
			: ((longAt((void *)(oop))) & (classIndexMask())) == ClassFloatCompactIndex);
}