/* Extracted from interp.c:45621 (function slidingCompactionInProgress). */

	/* SpurMemoryManager>>#slidingCompactionInProgress */

static int
slidingCompactionInProgress(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return GIV(gcPhaseInProgress) == SlidingCompactionInProgress;
}