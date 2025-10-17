/* Extracted from interp.c:28365 (function isInRememberedSet). */

	/* SpurGenerationScavenger>>#isInRememberedSet: */

static NoDbgRegParms sqInt
isInRememberedSet(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;

	for (i = 0; i < rememberedSetSize; i += 1) {
		if ((rememberedSet[i]) == objOop) {
			return 1;
		}
	}
	return 0;
}