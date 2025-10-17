/* Extracted from interp.c:28505 (function printRememberedSet). */

/*	Print the objects in the remembered set. */
/*	useful for debugging */

	/* SpurGenerationScavenger>>#printRememberedSet */

void
printRememberedSet(void)
{
    sqInt i;

	for (i = 0; i < rememberedSetSize; i += 1) {
		printNum(i);

		/* begin space */
		printChar(' ');
		shortPrintOop(rememberedSet[i]);
	}
}