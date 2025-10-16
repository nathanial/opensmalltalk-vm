/* Extracted from interp.c:68525 (function primitiveFlushCacheBySelector). */

static void
primitiveFlushCacheBySelector(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    sqInt probe;
    sqInt selector;

	selector = longAt(GIV(stackPointer));
	probe = 0;
	for (i = 1; i <= MethodCacheEntries; i += 1) {
		if ((GIV(methodCache)[probe + MethodCacheSelector]) == selector) {
			GIV(methodCache)[probe + MethodCacheSelector] = 0;
		}
		probe += MethodCacheEntrySize;
	}
	if ((selector == (longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SpecialSelectors) << (shiftForWord()))))))) + BaseHeaderSize) + (32U << (shiftForWord()))))))
	 || (selector == (longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SpecialSelectors) << (shiftForWord()))))))) + BaseHeaderSize) + (34U << (shiftForWord()))))))) {
		/* begin flushAtCache */
		memset(GIV(atCache), 0, AtCacheTotalSize * (sizeof(GIV(atCache)[0])));
	}
}