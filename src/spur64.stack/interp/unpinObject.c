/* Extracted from interp.c:46636 (function unpinObject). */

sqInt
unpinObject(sqInt objOop)
{
	assert(isNonImmediate(objOop));
	setIsPinnedOfto(objOop, 0);
	return 0;
}