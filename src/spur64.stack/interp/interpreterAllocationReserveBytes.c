/* Extracted from interp.c:54920 (function interpreterAllocationReserveBytes). */

/*	At a rough approximation we may need to allocate up to a couple
	of page's worth of contexts when switching stack pages, assigning
	to senders, etc. But the snapshot primitive voids all stack pages.
	So a safe margin is the size of a large context times the maximum
	number of frames per page times the number of pages. */

	/* StackInterpreter>>#interpreterAllocationReserveBytes */

static NeverInline sqInt
interpreterAllocationReserveBytes(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt maxFramesPerPage;
    sqInt maxUsedBytesPerPage;

	maxUsedBytesPerPage = (stackPageFrameBytes()) + (((FrameSlots + LargeContextSlots) + 1) * BytesPerWord);
	maxFramesPerPage = (maxUsedBytesPerPage / BytesPerWord) / FrameSlots;
	return ((maxFramesPerPage * LargeContextSlots) * BytesPerOop) * numStackPages;
}