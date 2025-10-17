/* Extracted from interp.c:68499 (function primitiveFlushCacheByMethod). */

/*	The receiver is a compiledMethod. Clear all entries in the method lookup
	cache that refer to this method, presumably because it has been redefined,
	overridden or removed.
 */

	/* StackInterpreterPrimitives>>#primitiveFlushCacheByMethod */

static void
primitiveFlushCacheByMethod(void)
{
    sqInt i;
    sqInt probe;

	/* begin flushMethodCacheForMethod: */
	probe = 0;
	for (i = 1; i <= MethodCacheEntries; i += 1) {
		if ((methodCache[probe + MethodCacheMethod]) == (longAt(stackPointer))) {
			methodCache[probe + MethodCacheSelector] = 0;
		}
		probe += MethodCacheEntrySize;
	}
	flushExternalPrimitiveOf(longAt(stackPointer));

	/* begin flushAtCache */
	memset(atCache, 0, AtCacheTotalSize * (sizeof(atCache[0])));
}