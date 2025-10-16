/* Extracted from interp.c:54967 (function isEmptyList). */

static NoDbgRegParms int
isEmptyList(sqInt aLinkedList)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(!(isForwarded(aLinkedList)));
	return (longAt((void *)((aLinkedList + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))))) == GIV(nilObj);
}