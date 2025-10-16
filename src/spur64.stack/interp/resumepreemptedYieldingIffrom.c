/* Extracted from interp.c:62237 (function resumepreemptedYieldingIffrom). */

static NoDbgRegParms sqInt
resumepreemptedYieldingIffrom(sqInt aProcess, sqInt yieldImplicitly, sqInt sourceCode)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt activePriority;
    sqInt activeProc;
    sqInt newPriority;
    sqInt objOop;
    sqInt oop;

	/* begin activeProcess */
	objOop = longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));
	activeProc = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));

	/* begin quickFetchInteger:ofObject: */
	oop = longAt((void *)((activeProc + BaseHeaderSize) + ((((usqInt)(PriorityIndex) << (shiftForWord()))))));
	assert((((oop) & 7) == 1));
	activePriority = (oop >> 3);

	/* begin quickFetchInteger:ofObject: */
	oop = longAt((void *)((aProcess + BaseHeaderSize) + ((((usqInt)(PriorityIndex) << (shiftForWord()))))));
	assert((((oop) & 7) == 1));
	newPriority = (oop >> 3);
	if (newPriority <= activePriority) {
		putToSleepyieldingIf(aProcess, 1);
		return 0;
	}
	putToSleepyieldingIf(activeProc, yieldImplicitly);
	transferTofrom(aProcess, sourceCode);
	return 1;
}