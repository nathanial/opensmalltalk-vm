/* Extracted from interp.c:47990 (function savedFirstFieldsSpaceWasAllocated). */

	/* SpurPlanningCompactor>>#savedFirstFieldsSpaceWasAllocated */

static sqInt
savedFirstFieldsSpaceWasAllocated(void)
{
	return savedFirstFieldsSpaceNotInOldSpace
	 && (oopisGreaterThan((savedFirstFieldsSpace.start), nilObj));
}