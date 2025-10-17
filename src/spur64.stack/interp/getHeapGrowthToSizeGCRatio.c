/* Extracted from interp.c:36024 (function getHeapGrowthToSizeGCRatio). */

	/* SpurMemoryManager>>#getHeapGrowthToSizeGCRatio */

static float
getHeapGrowthToSizeGCRatio(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return heapGrowthToSizeGCRatio;
}