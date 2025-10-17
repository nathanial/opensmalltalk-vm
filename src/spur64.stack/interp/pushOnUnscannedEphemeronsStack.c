/* Extracted from interp.c:44852 (function pushOnUnscannedEphemeronsStack). */

/*	Attempt to push anEphemeron on the unscanned ephemerons stack and answer
	if the attempt succeeded. Note that the ephemeron stack overflowing isn't
	a disaster; it simply means treating the ephemeron as strong in this GC
	cycle.  */

	/* SpurMemoryManager>>#pushOnUnscannedEphemeronsStack: */

static NoDbgRegParms sqInt
pushOnUnscannedEphemeronsStack(sqInt anEphemeron)
{
	assert(isEphemeron(anEphemeron));
	if (((unscannedEphemerons.top)) > ((unscannedEphemerons.limit))) {
		return 0;
	}
	longAtput((void *)((unscannedEphemerons.top = ((unscannedEphemerons.top)) + BytesPerOop)),anEphemeron);
	return 1;
}