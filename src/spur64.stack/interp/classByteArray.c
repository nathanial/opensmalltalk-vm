/* Extracted from interp.c:33070 (function classByteArray). */

/*	a.k.a. self fetchPointer: ClassByteArrayCompactIndex ofObject:
	classTableFirstPage 
 */

	/* SpurMemoryManager>>#classByteArray */

sqInt
classByteArray(void)
{
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassByteArray) << (shiftForWord()))))));
}