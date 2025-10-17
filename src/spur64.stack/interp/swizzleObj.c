/* Extracted from interp.c:49750 (function swizzleObj). */

	/* SpurSegmentManager>>#swizzleObj: */

static NoDbgRegParms sqInt
swizzleObj(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;

	assert(canSwizzle);
	for (i = (numSegments - 1); i >= 1; i += -1) {
		if (objOop >= (((segments[i]).segStart))) {
			return objOop + (((segments[i]).swizzle));
		}
	}
	return objOop + (((segments[0]).swizzle));
}