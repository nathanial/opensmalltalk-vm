/* Extracted from interp.c:35062 (function fetchLong32ofObject). */

/*	index by 32-bit units, and return a 32-bit value. Intended to replace
	fetchWord:ofObject: 
 */

	/* SpurMemoryManager>>#fetchLong32:ofObject: */

sqInt
fetchLong32ofObject(sqInt fieldIndex, sqInt oop)
{
	return long32At((void *)((oop + BaseHeaderSize) + ((((usqInt)(fieldIndex) << 2)))));
}