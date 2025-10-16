/* Extracted from interp.c:60916 (function printStackPageListInUse). */

void
printStackPageListInUse(void)
{
    sqInt n;
    StackPage *page;

	page = GIV(mostRecentlyUsedPage);
	n = 0;
	do {
		if (!(isFree(page))) {
			printStackPageuseCount(page, (n += 1));
			cr();
		}
	} while(((page = (page->prevPage))) != (GIV(mostRecentlyUsedPage)));
}