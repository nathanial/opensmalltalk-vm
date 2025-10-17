/* Extracted from interp.c:48858 (function useSegmentForSavedFirstFieldsSpace). */

/*	Attempt to allocate a memory segment large enough to hold the
	savedFirstFieldsSpace. Invoked when neither eden nor a large free chunk
	are found to be big enough for the job. */

	/* SpurPlanningCompactor>>#useSegmentForSavedFirstFieldsSpace: */

static NoDbgRegParms sqInt
useSegmentForSavedFirstFieldsSpace(sqInt spaceEstimate)
{
    usqInt allocatedSize;
    sqInt roundedSize;
    void *segAddress;

	allocatedSize = 0;
	roundedSize = ((spaceEstimate + 0x3FF) / 0x400) * 0x400;
	if ((segAddress = sqAllocateMemorySegmentOfSizeAboveAllocatedSizeInto(roundedSize, firstGapOfSizeAtLeast(roundedSize), (&allocatedSize)))) {
		(savedFirstFieldsSpace.start = ((usqIntptr_t)segAddress));
		(savedFirstFieldsSpace.limit = (((usqIntptr_t)segAddress)) + allocatedSize);
		savedFirstFieldsSpaceNotInOldSpace = 1;
		assert(savedFirstFieldsSpaceWasAllocated());
		return 1;
	}

	/* sent to the manager so that the simulator can increase memory to simulate a new segment */
	return 0;
}