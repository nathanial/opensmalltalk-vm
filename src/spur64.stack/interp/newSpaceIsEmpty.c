/* Extracted from interp.c:40759 (function newSpaceIsEmpty). */

static sqInt
newSpaceIsEmpty(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return (GIV(freeStart) == (((GIV(eden)).start)))
	 && (GIV(pastSpaceStart) == (((GIV(pastSpace)).start)));
}