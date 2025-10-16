/* Extracted from interp.c:54920 (function interpreterAllocationReserveBytes). */

static NeverInline sqInt
interpreterAllocationReserveBytes(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt maxFramesPerPage;
    sqInt maxUsedBytesPerPage;

	maxUsedBytesPerPage = (stackPageFrameBytes()) + (((FrameSlots + LargeContextSlots) + 1) * BytesPerWord);
	maxFramesPerPage = (maxUsedBytesPerPage / BytesPerWord) / FrameSlots;
	return ((maxFramesPerPage * LargeContextSlots) * BytesPerOop) * GIV(numStackPages);
}