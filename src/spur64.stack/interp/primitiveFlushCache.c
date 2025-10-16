/* Extracted from interp.c:16366 (function primitiveFlushCache). */

static void
primitiveFlushCache(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	/* begin flushMethodCache */
	memset(GIV(methodCache), 0, MethodCacheSize * (sizeof(GIV(methodCache)[0])));

	/* this for primitiveExternalMethod */
	GIV(lastMethodCacheProbeWrite) = 0;

	/* begin flushAtCache */
	memset(GIV(atCache), 0, AtCacheTotalSize * (sizeof(GIV(atCache)[0])));
}