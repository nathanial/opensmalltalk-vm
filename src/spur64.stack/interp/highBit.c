/* Extracted from interp.c:54668 (function highBit). */

sqInt
highBit(usqInt anUnsignedValue)
{
    sqInt bitNo;
    usqInt shifted;


	/* so it shows up in senders... */
	shifted = anUnsignedValue;
	bitNo = 0;
#  if BytesPerWord > 4
	if (!(shifted < (0x100000000LL))) {
		shifted = (shifted) >> 32;
		bitNo += 32;
	}
#  endif

	if (!(shifted < (0x10000))) {
		shifted = (shifted) >> 16;
		bitNo += 16;
	}
	if (!(shifted < (0x100))) {
		shifted = (shifted) >> 8;
		bitNo += 8;
	}
	if (!(shifted < (16))) {
		shifted = (shifted) >> 4;
		bitNo += 4;
	}
	if (!(shifted < (4))) {
		shifted = (shifted) >> 2;
		bitNo += 2;
	}
	if (!(shifted < (2))) {
		shifted = (shifted) >> 1;
		bitNo += 1;
	}

	/* shifted 0 or 1 now */
	return bitNo + shifted;
}