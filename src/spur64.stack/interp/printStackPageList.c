/* Extracted from interp.c:60899 (function printStackPageList). */

void
printStackPageList(void)
{
    StackPage *page;

	page = GIV(mostRecentlyUsedPage);
	do {
		/* begin printStackPage: */
		printStackPageuseCount(page, -1);
		cr();
	} while(((page = (page->prevPage))) != (GIV(mostRecentlyUsedPage)));
}