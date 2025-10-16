/* Extracted from interp.c:28505 (function printRememberedSet). */

void
printRememberedSet(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;

	for (i = 0; i < GIV(rememberedSetSize); i += 1) {
		printNum(i);

		/* begin space */
		printChar(' ');
		shortPrintOop(GIV(rememberedSet)[i]);
	}
}