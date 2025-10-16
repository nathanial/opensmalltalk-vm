/* Extracted from interp.c:64264 (function stackLimitBytes). */

static sqInt
stackLimitBytes(void)
{
	return (((stackPageFrameBytes()) < (((stackPageByteSize()) - (((FrameSlots + LargeContextSlots) + 1) * BytesPerWord)))) ? (stackPageFrameBytes()) : (((stackPageByteSize()) - (((FrameSlots + LargeContextSlots) + 1) * BytesPerWord))));
}