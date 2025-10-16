/* Extracted from interp.c:41186 (function numSlotsForBytes). */

static NoDbgRegParms sqInt
numSlotsForBytes(sqInt numBytes)
{
	return (numBytes + (BytesPerWord - 1)) / BytesPerWord;
}