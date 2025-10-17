/* Extracted from interp.c:11020 (function statAverageLivePagesWhenMapping). */

	/* CogStackPages>>#statAverageLivePagesWhenMapping */

static double
statAverageLivePagesWhenMapping(void)
{
	return (statNumMaps
			? (((double) statPageCountWhenMappingSum )) / statNumMaps
			: 0.0);
}