/* Extracted from interp.c:31839 (function bytesPerElement). */

unsigned char
bytesPerElement(sqInt oop)
{
    unsigned char byteSizes[16] =
								   {	wordSize(),  wordSize(),  wordSize(),  wordSize(),
									wordSize(),  wordSize(),  wordSize(),  0, /* forwarder */ wordSize(),
									8,
									4, 4,
									2, 2, 2, 2 };
    sqInt fmt;

	if (((oop & (tagMask())) != 0)) {
		return 0;
	}
	fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
	if (fmt >= (firstByteFormat())) {
		if (fmt >= (firstCompiledMethodFormat())) {
			return 0;
		}
		return 1;
	}
	return byteSizes[fmt];
}