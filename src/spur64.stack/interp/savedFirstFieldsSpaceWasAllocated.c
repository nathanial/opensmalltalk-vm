/* Extracted from interp.c:47990 (function savedFirstFieldsSpaceWasAllocated). */

static sqInt
savedFirstFieldsSpaceWasAllocated(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return GIV(savedFirstFieldsSpaceNotInOldSpace)
	 && (oopisGreaterThan((GIV(savedFirstFieldsSpace).start), GIV(nilObj)));
}