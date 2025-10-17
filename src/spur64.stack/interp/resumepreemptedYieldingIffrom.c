/* Extracted from interp.c:62237 (function resumepreemptedYieldingIffrom). */

/*	Make aProcess runnable and if its priority is higher than that of the
	current process, preempt the current process. Answer if the current
	process was preempted. If the current process was preempted then if
	yieldImplicitly add the current process to the back of its run queue,
	causing an implicit yiled to other processes on the run queue, otherwise
	add the current process to the front of its run queue, hence not yielding.
	Blue book behaviour is to yield implicitly but is arguably incorrect. */

	/* StackInterpreter>>#resume:preemptedYieldingIf:from: */

static NoDbgRegParms sqInt
resumepreemptedYieldingIffrom(sqInt aProcess, sqInt yieldImplicitly, sqInt sourceCode)
{
    sqInt activePriority;
    sqInt activeProc;
    sqInt newPriority;
    sqInt objOop;
    sqInt oop;

	/* begin activeProcess */
	objOop = longAt((void *)(((longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));
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