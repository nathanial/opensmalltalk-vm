/* Extracted from interp.c:49750 (function swizzleObj). */

static NoDbgRegParms sqInt
swizzleObj(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;

	assert(GIV(canSwizzle));
	for (i = (GIV(numSegments) - 1); i >= 1; i += -1) {
		if (objOop >= (((GIV(segments)[i]).segStart))) {
			return objOop + (((GIV(segments)[i]).swizzle));
		}
	}
	return objOop + (((GIV(segments)[0]).swizzle));
}