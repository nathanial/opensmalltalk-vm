/* Extracted from interp.c:46780 (function validFreeTreeChunk). */

	/* SpurMemoryManager>>#validFreeTreeChunk: */

static NoDbgRegParms sqInt
validFreeTreeChunk(sqInt chunk)
{
	if (!(segmentContainingObj(chunk))) {
		return 0;
	}
	return !(validFreeTreeChunkparent(chunk, longAt((void *)((chunk + BaseHeaderSize) + (2U << (shiftForWord()))))));
}