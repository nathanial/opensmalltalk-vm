/* Extracted from interp.c:31254 (function becomewith). */

sqInt
becomewith(sqInt array1, sqInt array2)
{
	return becomewithtwoWaycopyHash(array1, array2, 1, 1);
}