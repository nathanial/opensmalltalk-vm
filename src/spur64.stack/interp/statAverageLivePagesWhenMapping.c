/* Extracted from interp.c:11020 (function statAverageLivePagesWhenMapping). */

static double
statAverageLivePagesWhenMapping(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return (GIV(statNumMaps)
			? (((double) GIV(statPageCountWhenMappingSum) )) / GIV(statNumMaps)
			: 0.0);
}