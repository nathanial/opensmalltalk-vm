/* Extracted from interp.c:28869 (function remember). */

sqInt
remember(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(isNonImmediate(objOop));
	assert(!((isYoungObject(objOop))));
	assert(!((isRemembered(objOop))));
	assert(!((isInRememberedSet(objOop))));
	setIsRememberedOfto(objOop, 1);
	if (GIV(rememberedSetSize) >= GIV(rememberedSetLimit)) {
		growRememberedSet();
	}
	GIV(rememberedSet)[GIV(rememberedSetSize)] = objOop;
	if (((GIV(rememberedSetSize) += 1)) >= GIV(rememberedSetRedZone)) {
		/* begin scheduleScavenge */
		GIV(needGCFlag) = 1;
		forceInterruptCheck();
	}
	return objOop;
}