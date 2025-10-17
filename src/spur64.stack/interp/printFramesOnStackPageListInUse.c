/* Extracted from interp.c:59766 (function printFramesOnStackPageListInUse). */

/*	useful for VM debugging */

	/* StackInterpreter>>#printFramesOnStackPageListInUse */

void
printFramesOnStackPageListInUse(void)
{
    StackPage *page;

	page = GIV(mostRecentlyUsedPage);
	do {
		if (!(isFree(page))) {
			print("page ");
			printHexPtrnp(page);
			cr();
			printFramesInPage(page);
			cr();
		}
	} while(((page = (page->prevPage))) != (GIV(mostRecentlyUsedPage)));
}