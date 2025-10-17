/* Extracted from interp.c:25929 (function signalNoResume). */

/*	Signal the given semaphore from within the interpreter. Used to serialize
	callbacks. 
 */

	/* InterpreterPrimitives>>#signalNoResume: */

int
signalNoResume(sqInt aSemaphore)
{
    int empty;

	/* begin isEmptyList: */
	assert(!(isForwarded(aSemaphore)));
	empty = (longAt((void *)((aSemaphore + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))))) == nilObj;
	if (!empty) {
		putToSleepyieldingIf(removeFirstLinkOfList(aSemaphore), 1);
	}
	return empty;
}