/* Extracted from interp.c:33290 (function classLargePositiveInteger). */

sqInt
classLargePositiveInteger(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	/* begin knownClassAtIndex: */
	assert(((ClassLargePositiveIntegerCompactIndex >= 1) && (ClassLargePositiveIntegerCompactIndex <= (classTablePageSize()))));
	return longAt((void *)((GIV(classTableFirstPage) + BaseHeaderSize) + ((((usqInt)(ClassLargePositiveIntegerCompactIndex) << (shiftForWord()))))));
}