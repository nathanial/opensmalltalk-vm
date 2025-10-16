/* Extracted from interp.c:49248 (function isInSegments). */

static NoDbgRegParms sqInt
isInSegments(usqInt address)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;

	for (i = 0; i < GIV(numSegments); i += 1) {
		if (address < (((GIV(segments)[i]).segStart))) {
			return 0;
		}
		if (address < ((((GIV(segments)[i]).segSize)) + (((GIV(segments)[i]).segStart)))) {
			return 1;
		}
	}
	return 0;
}