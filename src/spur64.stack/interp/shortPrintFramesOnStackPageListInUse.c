/* Extracted from interp.c:63219 (function shortPrintFramesOnStackPageListInUse). */

/*	useful for VM debugging */

	/* StackInterpreter>>#shortPrintFramesOnStackPageListInUse */

void
shortPrintFramesOnStackPageListInUse(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    StackPage *page;

	page = mostRecentlyUsedPage;
	do {
		if (!(isFree(page))) {
			/* begin print: */
			fprintf(transcript,
					"%s",
					"page ");
			printHexPtrnp(page);
			cr();
			shortPrintFramesInPage(page);
			cr();
		}
	} while(((page = (page->prevPage))) != (mostRecentlyUsedPage));
}