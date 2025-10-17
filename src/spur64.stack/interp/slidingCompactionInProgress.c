/* Extracted from interp.c:45621 (function slidingCompactionInProgress). */

	/* SpurMemoryManager>>#slidingCompactionInProgress */

static int
slidingCompactionInProgress(void)
{
	return gcPhaseInProgress == SlidingCompactionInProgress;
}