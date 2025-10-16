/* Extracted from interp.c:37168 (function isCharacterObject). */

int
isCharacterObject(sqInt oop)
{
	return ((oop & (characterTag())) != 0);
}