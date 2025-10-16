/* Extracted from interp.c:37465 (function isNonImmediate). */

int
isNonImmediate(sqInt oop)
{
	return (!(oop & (tagMask())));
}