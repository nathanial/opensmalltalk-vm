/* Extracted from interp.c:16366 (function primitiveFlushCache). */

/*	Clear the method lookup cache. This must be done after every programming
	change. 
 */

	/* InterpreterPrimitives>>#primitiveFlushCache */

static void
primitiveFlushCache(void)
{
	/* begin flushMethodCache */
	memset(methodCache, 0, MethodCacheSize * (sizeof(methodCache[0])));

	/* this for primitiveExternalMethod */
	lastMethodCacheProbeWrite = 0;

	/* begin flushAtCache */
	memset(atCache, 0, AtCacheTotalSize * (sizeof(atCache[0])));
}