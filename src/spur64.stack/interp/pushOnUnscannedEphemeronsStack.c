/* Extracted from interp.c:44852 (function pushOnUnscannedEphemeronsStack). */

static NoDbgRegParms sqInt
pushOnUnscannedEphemeronsStack(sqInt anEphemeron)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(isEphemeron(anEphemeron));
	if (((GIV(unscannedEphemerons).top)) > ((GIV(unscannedEphemerons).limit))) {
		return 0;
	}
	longAtput((void *)((GIV(unscannedEphemerons).top = ((GIV(unscannedEphemerons).top)) + BytesPerOop)),anEphemeron);
	return 1;
}