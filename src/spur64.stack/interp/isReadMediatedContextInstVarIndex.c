/* Extracted from interp.c:55168 (function isReadMediatedContextInstVarIndex). */

int
isReadMediatedContextInstVarIndex(sqInt index)
{
	return index <= StackPointerIndex;
}