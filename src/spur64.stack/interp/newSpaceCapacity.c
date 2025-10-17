/* Extracted from interp.c:28456 (function newSpaceCapacity). */

	/* SpurGenerationScavenger>>#newSpaceCapacity */

static usqInt
newSpaceCapacity(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return ((eden.limit)) - (((((futureSpace.start)) < ((pastSpace.start))) ? ((futureSpace.start)) : ((pastSpace.start))));
}