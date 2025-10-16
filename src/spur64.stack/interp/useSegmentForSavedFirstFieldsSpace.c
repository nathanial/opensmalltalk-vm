/* Extracted from interp.c:48858 (function useSegmentForSavedFirstFieldsSpace). */

static NoDbgRegParms sqInt
useSegmentForSavedFirstFieldsSpace(sqInt spaceEstimate)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt allocatedSize;
    sqInt roundedSize;
    void *segAddress;

	allocatedSize = 0;
	roundedSize = ((spaceEstimate + 0x3FF) / 0x400) * 0x400;
	if ((segAddress = sqAllocateMemorySegmentOfSizeAboveAllocatedSizeInto(roundedSize, firstGapOfSizeAtLeast(roundedSize), (&allocatedSize)))) {
		(GIV(savedFirstFieldsSpace).start = ((usqIntptr_t)segAddress));
		(GIV(savedFirstFieldsSpace).limit = (((usqIntptr_t)segAddress)) + allocatedSize);
		GIV(savedFirstFieldsSpaceNotInOldSpace) = 1;
		assert(savedFirstFieldsSpaceWasAllocated());
		return 1;
	}

	/* sent to the manager so that the simulator can increase memory to simulate a new segment */
	return 0;
}