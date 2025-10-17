/* Extracted from interp.c:37914 (function isYoungObject). */

/*	Answer if obj is young. Require that obj is non-immediate. */

	/* SpurMemoryManager>>#isYoungObject: */

int
isYoungObject(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(isNonImmediate(objOop));
	return oopisLessThan(objOop, oldSpaceStart);
}