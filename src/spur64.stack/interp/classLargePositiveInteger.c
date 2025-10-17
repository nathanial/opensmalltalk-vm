/* Extracted from interp.c:33290 (function classLargePositiveInteger). */

	/* SpurMemoryManager>>#classLargePositiveInteger */

sqInt
classLargePositiveInteger(void)
{
	/* begin knownClassAtIndex: */
	assert(((ClassLargePositiveIntegerCompactIndex >= 1) && (ClassLargePositiveIntegerCompactIndex <= (classTablePageSize()))));
	return longAt((void *)((classTableFirstPage + BaseHeaderSize) + ((((usqInt)(ClassLargePositiveIntegerCompactIndex) << (shiftForWord()))))));
}