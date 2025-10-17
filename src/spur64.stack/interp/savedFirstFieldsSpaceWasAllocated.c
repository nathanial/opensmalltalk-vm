/* Extracted from interp.c:47990 (function savedFirstFieldsSpaceWasAllocated). */

	/* SpurPlanningCompactor>>#savedFirstFieldsSpaceWasAllocated */

static sqInt
savedFirstFieldsSpaceWasAllocated(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return savedFirstFieldsSpaceNotInOldSpace
	 && (oopisGreaterThan((savedFirstFieldsSpace.start), nilObj));
}