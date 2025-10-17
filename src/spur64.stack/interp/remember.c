/* Extracted from interp.c:28869 (function remember). */

/*	Add the argument to the remembered set and set its isRemembered bit to
	true. Answer the argument for the benefit of the Cogit. */

	/* SpurGenerationScavenger>>#remember: */

sqInt
remember(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(isNonImmediate(objOop));
	assert(!((isYoungObject(objOop))));
	assert(!((isRemembered(objOop))));
	assert(!((isInRememberedSet(objOop))));
	setIsRememberedOfto(objOop, 1);
	if (rememberedSetSize >= rememberedSetLimit) {
		growRememberedSet();
	}
	rememberedSet[rememberedSetSize] = objOop;
	if (((rememberedSetSize += 1)) >= rememberedSetRedZone) {
		/* begin scheduleScavenge */
		needGCFlag = 1;
		forceInterruptCheck();
	}
	return objOop;
}