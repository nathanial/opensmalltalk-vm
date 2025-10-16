/* Extracted from interp.c:29397 (function scavengerTenuringThreshold). */

static float
scavengerTenuringThreshold(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return (GIV(tenureThreshold) >= ((GIV(pastSpace).start))
			? (((double) (GIV(tenureThreshold) - ((GIV(pastSpace).start))) )) / (((GIV(pastSpace).limit)) - ((GIV(pastSpace).start)))
			: 0);
}