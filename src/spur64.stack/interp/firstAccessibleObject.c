/* Extracted from interp.c:35335 (function firstAccessibleObject). */

	/* SpurMemoryManager>>#firstAccessibleObject */

static sqInt
firstAccessibleObject(void)
{
    sqInt savedTenuringThreshold;

	assert(nilObj == oldSpaceStart);

	/* flush newSpace to settle the enumeration. */

	/* begin flushNewSpace */
	savedTenuringThreshold = tenureThreshold;
	tenureThreshold = oldSpaceStart;
	scavengingGCTenuringIf(TenureByAge);

	/* begin setRawTenuringThreshold: */
	tenureThreshold = savedTenuringThreshold;
	assert((rememberedSetSize) == 0);
	assert(pastSpaceStart == (((pastSpace).start)));
	assert(freeStart == (((eden).start)));
	return nilObj;
}