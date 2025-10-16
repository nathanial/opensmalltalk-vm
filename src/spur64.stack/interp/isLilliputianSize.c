/* Extracted from interp.c:27368 (function isLilliputianSize). */

static NoDbgRegParms int
isLilliputianSize(sqInt chunkBytes)
{
	assert(chunkBytes >= (BaseHeaderSize + (allocationUnit())));
	return chunkBytes == (BaseHeaderSize + 8 /* allocationUnit */);
}