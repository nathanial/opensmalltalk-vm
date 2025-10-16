/* Extracted from interp.c:25929 (function signalNoResume). */

int
signalNoResume(sqInt aSemaphore)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    int empty;

	/* begin isEmptyList: */
	assert(!(isForwarded(aSemaphore)));
	empty = (longAt((void *)((aSemaphore + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))))) == GIV(nilObj);
	if (!empty) {
		putToSleepyieldingIf(removeFirstLinkOfList(aSemaphore), 1);
	}
	return empty;
}