/* Extracted from interp.c:62696 (function rewriteMethodCacheEntryForExternalPrimitiveToFunction). */

static NoDbgRegParms void
rewriteMethodCacheEntryForExternalPrimitiveToFunction(void (*localPrimAddress)())
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if ((GIV(methodCache)[GIV(lastMethodCacheProbeWrite) + MethodCacheMethod]) == GIV(newMethod)) {
		GIV(methodCache)[GIV(lastMethodCacheProbeWrite) + MethodCachePrimFunction] = (((sqIntptr_t) localPrimAddress));
	}
}