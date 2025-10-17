/* Extracted from interp.c:37500 (function isOldObject). */

/*	Answer if obj is old. Require that obj is non-immediate. */

	/* SpurMemoryManager>>#isOldObject: */

static NoDbgRegParms int
isOldObject(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(isNonImmediate(objOop));
	return oopisGreaterThanOrEqualTo(objOop, GIV(oldSpaceStart));
}