/* Extracted from interp.c:43273 (function printFreeListHeads). */

void
printFreeListHeads(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt expectedMask;
    sqInt i;

	expectedMask = 0;
	for (i = 0; i <= 0x3F /* (numFreeLists - 1) */; i += 1) {
		printHex(GIV(freeLists)[i]);
		if (GIV(freeLists)[i]) {
			expectedMask += 1ULL << i;
		}
		if ((i + 1) % (4)) {
			print("  ");
		}
		else {
			cr();
		}
	}
	cr();
	print("mask: ");

	/* printHexnp: */
	fprintf(GIV(transcript),
			"%p",
			((void *)GIV(freeListsMask)));
	print(" expected: ");

	/* printHexnp: */
	fprintf(GIV(transcript),
			"%p",
			((void *)expectedMask));
	cr();
}