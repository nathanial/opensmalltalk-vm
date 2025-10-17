/* Extracted from interp.c:36464 (function initializeNewSpaceVariables). */

	/* SpurMemoryManager>>#initializeNewSpaceVariables */

static NeverInline void
initializeNewSpaceVariables(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	GIV(freeStart) = ((GIV(eden)).start);
	GIV(pastSpaceStart) = ((GIV(pastSpace)).start);
	GIV(scavengeThreshold) = ((((GIV(eden)).limit)) - ((((GIV(eden).limit)) - ((GIV(eden).start))) / 64)) - (interpreterAllocationReserveBytes());
	GIV(newSpaceStart) = (((((GIV(pastSpace)).start)) < (((GIV(futureSpace)).start))) ? (((GIV(pastSpace)).start)) : (((GIV(futureSpace)).start)));
	assert(GIV(newSpaceStart) < (((GIV(eden)).start)));
}