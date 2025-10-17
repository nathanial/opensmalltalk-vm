/* Extracted from interp.c:62696 (function
 * rewriteMethodCacheEntryForExternalPrimitiveToFunction). */

/*	Rewrite an existing entry in the method cache with a new primitive
        function address.
        Used by primitiveExternalCall to make direct calls to found external
        prims, or quickly
        fail not found external prims. */

/* StackInterpreter>>#rewriteMethodCacheEntryForExternalPrimitiveToFunction: */

static NoDbgRegParms void rewriteMethodCacheEntryForExternalPrimitiveToFunction(
    void (*localPrimAddress)()) {
  if ((methodCache[lastMethodCacheProbeWrite + MethodCacheMethod]) ==
      newMethod) {
    methodCache[lastMethodCacheProbeWrite + MethodCachePrimFunction] =
        (((sqIntptr_t)localPrimAddress));
  }
}