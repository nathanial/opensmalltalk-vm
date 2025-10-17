/* Extracted from interp.c:26866 (function bytesInBody). */

/*	Answer the total number of bytes in an object including header and
	possible overflow size header.
 */

	/* Spur64BitMemoryManager>>#bytesInBody: */

static NoDbgRegParms usqInt
bytesInBody(sqInt objOop)
{
    usqLong header;
    usqLong headerNumSlots;
    usqInt numSlots;

	header = longAt((void *)(objOop));
	headerNumSlots = (header) >> (numSlotsFullShift());
	numSlots = (headerNumSlots == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize)))) << 8)))))) >> 8
				: (headerNumSlots
						? headerNumSlots
						: 1));
	return ((numSlots << (shiftForWord()))) + ((headerNumSlots == (numSlotsMask())
		? BaseHeaderSize + BaseHeaderSize
		: BaseHeaderSize));
}