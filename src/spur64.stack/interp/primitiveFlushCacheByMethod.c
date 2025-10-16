/* Extracted from interp.c:68499 (function primitiveFlushCacheByMethod). */

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