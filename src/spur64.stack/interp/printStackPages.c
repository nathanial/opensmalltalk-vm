/* Extracted from interp.c:60936 (function printStackPages). */

void
printStackPages(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    StackPage *page;

	for (i = 0; i < GIV(numStackPages); i += 1) {
		/* begin stackPageAt: */
		page = stackPageAtpages(i, GIV(pages));
		printStackPageuseCount(page, -1);
		cr();
	}
}