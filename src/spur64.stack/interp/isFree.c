/* Extracted from interp.c:10805 (function isFree). */

static NoDbgRegParms int
isFree(StackPage *self_in_CogStackPage)
{
	return ((self_in_CogStackPage->baseFP)) == 0;
}