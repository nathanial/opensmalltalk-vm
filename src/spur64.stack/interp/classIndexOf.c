/* Extracted from interp.c:33274 (function classIndexOf). */

sqInt
classIndexOf(sqInt objOop)
{
	return (longAt((void *)(objOop))) & (classIndexMask());
}