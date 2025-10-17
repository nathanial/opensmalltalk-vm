/* Extracted from interp.c:43273 (function printFreeListHeads). */

/*	useful for VM debugging */

	/* SpurMemoryManager>>#printFreeListHeads */

void
printFreeListHeads(void)
{
    sqInt expectedMask;
    sqInt i;

	expectedMask = 0;
	for (i = 0; i <= 0x3F /* (numFreeLists - 1) */; i += 1) {
		printHex(freeLists[i]);
		if (freeLists[i]) {
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
	fprintf(transcript,
			"%p",
			((void *)freeListsMask));
	print(" expected: ");

	/* printHexnp: */
	fprintf(transcript,
			"%p",
			((void *)expectedMask));
	cr();
}