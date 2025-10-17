/* Extracted from interp.c:28233 (function firstCorpse). */

	/* SpurGenerationScavenger>>#firstCorpse: */

static NoDbgRegParms usqInt
firstCorpse(sqInt headOfCorpseList)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return ((((usqInt)((headOfCorpseList - 1)) << 3 /* shiftForAllocationUnit */))) + GIV(newSpaceStart);
}