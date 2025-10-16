/* Extracted from interp.c:27340 (function isIntegerObject). */

int
isIntegerObject(sqInt oop)
{
	return ((oop & (smallIntegerTag())) != 0);
}