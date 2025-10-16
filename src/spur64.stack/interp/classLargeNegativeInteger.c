/* Extracted from interp.c:33281 (function classLargeNegativeInteger). */

sqInt
classLargeNegativeInteger(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	/* begin knownClassAtIndex: */
	assert(((ClassLargeNegativeIntegerCompactIndex >= 1) && (ClassLargeNegativeIntegerCompactIndex <= (classTablePageSize()))));
	return longAt((void *)((GIV(classTableFirstPage) + BaseHeaderSize) + ((((usqInt)(ClassLargeNegativeIntegerCompactIndex) << (shiftForWord()))))));
}