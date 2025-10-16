/* Extracted from interp.c:35062 (function fetchLong32ofObject). */

sqInt
fetchLong32ofObject(sqInt fieldIndex, sqInt oop)
{
	return long32At((void *)((oop + BaseHeaderSize) + ((((usqInt)(fieldIndex) << 2)))));
}