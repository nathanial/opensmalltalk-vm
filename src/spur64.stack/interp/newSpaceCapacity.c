/* Extracted from interp.c:28456 (function newSpaceCapacity). */

	/* SpurGenerationScavenger>>#newSpaceCapacity */

static usqInt
newSpaceCapacity(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return ((GIV(eden).limit)) - (((((GIV(futureSpace).start)) < ((GIV(pastSpace).start))) ? ((GIV(futureSpace).start)) : ((GIV(pastSpace).start))));
}