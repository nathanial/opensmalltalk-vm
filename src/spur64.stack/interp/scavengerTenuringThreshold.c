/* Extracted from interp.c:29397 (function scavengerTenuringThreshold). */

/*	(Slang flattens so need unique selectors) */

	/* SpurGenerationScavenger>>#scavengerTenuringThreshold */

static float
scavengerTenuringThreshold(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return (tenureThreshold >= ((pastSpace.start))
			? (((double) (tenureThreshold - ((pastSpace.start))) )) / (((pastSpace.limit)) - ((pastSpace.start)))
			: 0);
}