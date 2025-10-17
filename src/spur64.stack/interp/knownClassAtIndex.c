/* Extracted from interp.c:38002 (function knownClassAtIndex). */

	/* SpurMemoryManager>>#knownClassAtIndex: */

static NoDbgRegParms sqInt
knownClassAtIndex(sqInt classIndex)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(((classIndex >= 1) && (classIndex <= (classTablePageSize()))));
	return longAt((void *)((classTableFirstPage + BaseHeaderSize) + ((((usqInt)(classIndex) << (shiftForWord()))))));
}