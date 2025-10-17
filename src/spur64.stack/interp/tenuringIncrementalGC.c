/* Extracted from interp.c:46219 (function tenuringIncrementalGC). */

/*	Do an incremental GC that tenures all surviving young objects to old
	space. The selector tenuringIncrementalGC refers to the V3 incremental
	collector, which is the collector for young objects, equivalent in
	function to the scavenger.
	The Spur incremental collector is called the global incremental garbage
	collector or globalIGC for short.
 */

	/* SpurMemoryManager>>#tenuringIncrementalGC */

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