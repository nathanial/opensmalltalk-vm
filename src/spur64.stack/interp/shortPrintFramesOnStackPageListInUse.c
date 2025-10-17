/* Extracted from interp.c:63219 (function shortPrintFramesOnStackPageListInUse). */

/*	useful for VM debugging */

	/* StackInterpreter>>#shortPrintFramesOnStackPageListInUse */

void
shortPrintFramesOnStackPageListInUse(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    StackPage *page;

	page = GIV(mostRecentlyUsedPage);
	do {
		if (!(isFree(page))) {
			/* begin print: */
			fprintf(GIV(transcript),
					"%s",
					"page ");
			printHexPtrnp(page);
			cr();
			shortPrintFramesInPage(page);
			cr();
		}
	} while(((page = (page->prevPage))) != (GIV(mostRecentlyUsedPage)));
}