/* Extracted from interp.c:46219 (function tenuringIncrementalGC). */

void
tenuringIncrementalGC(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt savedTenuringThreshold;

	/* begin flushNewSpace */
	savedTenuringThreshold = GIV(tenureThreshold);
	GIV(tenureThreshold) = GIV(oldSpaceStart);
	scavengingGCTenuringIf(TenureByAge);

	/* begin setRawTenuringThreshold: */
	GIV(tenureThreshold) = savedTenuringThreshold;
	assert((GIV(rememberedSetSize)) == 0);
	assert(GIV(pastSpaceStart) == (((GIV(pastSpace)).start)));
	assert(GIV(freeStart) == (((GIV(eden)).start)));
}