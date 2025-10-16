/* Extracted from interp.c:40667 (function maxIdentityHash). */

sqInt
maxIdentityHash(void)
{
	return identityHashHalfWordMask();
}