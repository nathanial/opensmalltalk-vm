/* Extracted from interp.c:55426 (function isWriteMediatedContextInstVarIndex). */

int
isWriteMediatedContextInstVarIndex(sqInt index)
{
	return index <= ReceiverIndex;
}