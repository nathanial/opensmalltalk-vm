/* Extracted from interp.c:35036 (function fetchClassOf). */

sqInt
fetchClassOf(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt tagBits;

	return ((tagBits = oop & (tagMask()))
			? longAt((void *)((GIV(classTableFirstPage) + BaseHeaderSize) + ((((usqInt)(tagBits) << (shiftForWord()))))))
			: fetchClassOfNonImm(oop));
}