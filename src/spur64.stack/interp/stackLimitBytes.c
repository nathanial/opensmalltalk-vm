/* Extracted from interp.c:64264 (function stackLimitBytes). */

/*	Answer the actual stackLimit offset in a page. Since stackPageByteSize may
	have chosen to shrink a page
	this may be less than stackPageFrameBytes, but it should be no more than
	stackPageFrameBytes. 
 */

	/* StackInterpreter>>#stackLimitBytes */

static sqInt
stackLimitBytes(void)
{
	return (((stackPageFrameBytes()) < (((stackPageByteSize()) - (((FrameSlots + LargeContextSlots) + 1) * BytesPerWord)))) ? (stackPageFrameBytes()) : (((stackPageByteSize()) - (((FrameSlots + LargeContextSlots) + 1) * BytesPerWord))));
}