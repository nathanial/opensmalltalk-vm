/* Extracted from interp.c:60936 (function printStackPages). */

/*	useful for VM debugging */

	/* StackInterpreter>>#printStackPages */

void
printStackPages(void)
{
    sqInt i;
    StackPage *page;

	for (i = 0; i < numStackPages; i += 1) {
		/* begin stackPageAt: */
		page = stackPageAtpages(i, pages);
		printStackPageuseCount(page, -1);
		cr();
	}
}