/* Extracted from interp.c:68499 (function primitiveFlushCacheByMethod). */

/*	The receiver is a compiledMethod. Clear all entries in the method lookup
	cache that refer to this method, presumably because it has been redefined,
	overridden or removed.
 */

	/* StackInterpreterPrimitives>>#primitiveFlushCacheByMethod */

static void
primitiveFlushCacheByMethod(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    sqInt probe;

	/* begin flushMethodCacheForMethod: */
	probe = 0;
	for (i = 1; i <= MethodCacheEntries; i += 1) {
		if ((GIV(methodCache)[probe + MethodCacheMethod]) == (longAt(GIV(stackPointer)))) {
			GIV(methodCache)[probe + MethodCacheSelector] = 0;
		}
		probe += MethodCacheEntrySize;
	}
	flushExternalPrimitiveOf(longAt(GIV(stackPointer)));

	/* begin flushAtCache */
	memset(GIV(atCache), 0, AtCacheTotalSize * (sizeof(GIV(atCache)[0])));
}