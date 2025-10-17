/* Extracted from interp.c:56065 (function lookupInMethodCacheSelclassTag). */

/*	This method implements a simple method lookup cache. If an entry for the
	given selector and classTag is found in the cache, set the values of
	'newMethod' and
	'primitiveFunctionPointer' and answer true. Otherwise, answer false. */

	/* StackInterpreter>>#lookupInMethodCacheSel:classTag: */

static NoDbgRegParms sqInt
lookupInMethodCacheSelclassTag(sqInt selector, sqInt classTag)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt hash;
    usqInt probe;

	/* begin inlineLookupInMethodCacheSel:classTag: */
	/* shift drops two low-order zeros from addresses */
	hash = selector ^ ((((usqInt)(classTag) << 2)));

	/* first probe */
	probe = hash & MethodCacheMask;
	if (((GIV(methodCache)[probe + MethodCacheSelector]) == selector)
	 && ((GIV(methodCache)[probe + MethodCacheClass]) == classTag)) {
		GIV(newMethod) = GIV(methodCache)[probe + MethodCacheMethod];
		primitiveFunctionPointer = ((void (*)()) (GIV(methodCache)[probe + MethodCachePrimFunction]));
		return 1;
	}

	/* second probe */
	probe = (((usqInt)(hash)) >> 1) & MethodCacheMask;
	if (((GIV(methodCache)[probe + MethodCacheSelector]) == selector)
	 && ((GIV(methodCache)[probe + MethodCacheClass]) == classTag)) {
		GIV(newMethod) = GIV(methodCache)[probe + MethodCacheMethod];
		primitiveFunctionPointer = ((void (*)()) (GIV(methodCache)[probe + MethodCachePrimFunction]));
		return 1;
	}
	probe = (((usqInt)(hash)) >> 2) & MethodCacheMask;
	if (((GIV(methodCache)[probe + MethodCacheSelector]) == selector)
	 && ((GIV(methodCache)[probe + MethodCacheClass]) == classTag)) {
		GIV(newMethod) = GIV(methodCache)[probe + MethodCacheMethod];
		primitiveFunctionPointer = ((void (*)()) (GIV(methodCache)[probe + MethodCachePrimFunction]));
		return 1;
	}
	return 0;
}