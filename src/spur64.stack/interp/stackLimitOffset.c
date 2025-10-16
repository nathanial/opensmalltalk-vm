/* Extracted from interp.c:64295 (function stackLimitOffset). */

static sqInt
stackLimitOffset(void)
{
	return ((FrameSlots + LargeContextSlots) + 1) * BytesPerWord;
}