/* Extracted from interp.c:35036 (function fetchClassOf). */

	/* SpurMemoryManager>>#fetchClassOf: */

sqInt
fetchClassOf(sqInt oop)
{
    sqInt tagBits;

	return ((tagBits = oop & (tagMask()))
			? longAt((void *)((classTableFirstPage + BaseHeaderSize) + ((((usqInt)(tagBits) << (shiftForWord()))))))
			: fetchClassOfNonImm(oop));
}