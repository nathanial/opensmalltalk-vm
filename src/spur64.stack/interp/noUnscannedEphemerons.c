/* Extracted from interp.c:41032 (function noUnscannedEphemerons). */

	/* SpurMemoryManager>>#noUnscannedEphemerons */

static int
noUnscannedEphemerons(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return ((GIV(unscannedEphemerons).top)) < ((GIV(unscannedEphemerons).start));
}