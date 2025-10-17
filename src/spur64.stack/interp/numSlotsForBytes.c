/* Extracted from interp.c:41186 (function numSlotsForBytes). */

	/* SpurMemoryManager>>#numSlotsForBytes: */

static NoDbgRegParms sqInt
numSlotsForBytes(sqInt numBytes)
{
	return (numBytes + (BytesPerWord - 1)) / BytesPerWord;
}