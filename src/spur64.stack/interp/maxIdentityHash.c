/* Extracted from interp.c:40667 (function maxIdentityHash). */

	/* SpurMemoryManager>>#maxIdentityHash */

sqInt
maxIdentityHash(void)
{
	return identityHashHalfWordMask();
}