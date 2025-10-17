/* Extracted from interp.c:64329 (function stackPageByteSize). */

/*	Answer a page size that is a power-of-two and contains a useful number of
	frames. Room for 256 slots for frames gives around 40 frames a page which
	is a
	good compromise between overflow rate and latency in divorcing a page. */

	/* StackInterpreter>>#stackPageByteSize */

static sqInt
stackPageByteSize(void)
{
    sqInt largeSize;
    sqInt pageBytes;
    sqInt smallSize;

	pageBytes = ((stackPageFrameBytes()) + (((FrameSlots + LargeContextSlots) + 1) * BytesPerWord));
	if (!(pageBytes & (pageBytes - 1))) {
		return pageBytes;
	}

	/* = 0 => a power of two
	   round up or round down; that is the question.  If rounding down reduces
	   the size by no more than 1/8th round down, otherwise roundup. */
	largeSize = 1ULL << (highBit(pageBytes));
	smallSize = 1ULL << ((highBit(pageBytes)) - 1);
	assert((largeSize > pageBytes)
	 && (pageBytes > smallSize));
	return ((pageBytes - smallSize) <= (smallSize / 8)
			? smallSize
			: largeSize);
}