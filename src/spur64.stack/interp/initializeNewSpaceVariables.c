/* Extracted from interp.c:36464 (function initializeNewSpaceVariables). */

	/* SpurMemoryManager>>#initializeNewSpaceVariables */

static NeverInline void
initializeNewSpaceVariables(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	freeStart = ((eden).start);
	pastSpaceStart = ((pastSpace).start);
	scavengeThreshold = ((((eden).limit)) - ((((eden.limit)) - ((eden.start))) / 64)) - (interpreterAllocationReserveBytes());
	newSpaceStart = (((((pastSpace).start)) < (((futureSpace).start))) ? (((pastSpace).start)) : (((futureSpace).start)));
	assert(newSpaceStart < (((eden).start)));
}