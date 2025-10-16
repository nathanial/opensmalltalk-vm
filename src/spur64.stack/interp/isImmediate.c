/* Extracted from interp.c:37326 (function isImmediate). */

int
isImmediate(sqInt oop)
{
	return ((oop & (tagMask())) != 0);
}