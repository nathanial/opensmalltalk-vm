/* Extracted from interp.c:10853 (function freeStackPage). */

static NoDbgRegParms void
freeStackPage(StackPage *aPage)
{
	freeStackPageNoAssert(aPage);
	assert(pageListIsWellFormed());
}