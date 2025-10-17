/* Extracted from interp.c:35335 (function firstAccessibleObject). */

	/* SpurMemoryManager>>#firstAccessibleObject */

static sqInt
firstAccessibleObject(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt savedTenuringThreshold;

	assert(GIV(nilObj) == GIV(oldSpaceStart));

	/* flush newSpace to settle the enumeration. */

	/* begin flushNewSpace */
	savedTenuringThreshold = GIV(tenureThreshold);
	GIV(tenureThreshold) = GIV(oldSpaceStart);
	scavengingGCTenuringIf(TenureByAge);

	/* begin setRawTenuringThreshold: */
	GIV(tenureThreshold) = savedTenuringThreshold;
	assert((GIV(rememberedSetSize)) == 0);
	assert(GIV(pastSpaceStart) == (((GIV(pastSpace)).start)));
	assert(GIV(freeStart) == (((GIV(eden)).start)));
	return GIV(nilObj);
}