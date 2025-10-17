/* Extracted from interp.c:33070 (function classByteArray). */

/*	a.k.a. self fetchPointer: ClassByteArrayCompactIndex ofObject:
	classTableFirstPage 
 */

	/* SpurMemoryManager>>#classByteArray */

sqInt
classByteArray(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassByteArray) << (shiftForWord()))))));
}