/* Extracted from interp.c:11020 (function statAverageLivePagesWhenMapping). */

	/* CogStackPages>>#statAverageLivePagesWhenMapping */

static double
statAverageLivePagesWhenMapping(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return (statNumMaps
			? (((double) statPageCountWhenMappingSum )) / statNumMaps
			: 0.0);
}