/* Extracted from interp.c:25893 (function primitiveYield). */

/*	Primitively do the equivalent of Process>yield, avoiding the overhead of a
	fork and a wait in the standard implementation.
 */

	/* InterpreterPrimitives>>#primitiveYield */

static void
primitiveYield(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt activeProc;
    sqInt oop;
    sqInt priority;
    sqInt processList;
    sqInt processLists;
    sqInt scheduler;

	scheduler = longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));
	activeProc = longAt((void *)((scheduler + BaseHeaderSize) + ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));

	/* begin quickFetchInteger:ofObject: */
	oop = longAt((void *)((activeProc + BaseHeaderSize) + ((((usqInt)(PriorityIndex) << (shiftForWord()))))));
	assert((((oop) & 7) == 1));
	priority = (oop >> 3);
	processLists = longAt((void *)((scheduler + BaseHeaderSize) + ((((usqInt)(ProcessListsIndex) << (shiftForWord()))))));
	processList = longAt((void *)((processLists + BaseHeaderSize) + ((((usqInt)((priority - 1)) << (shiftForWord()))))));

	/* begin isEmptyList: */
	assert(!(isForwarded(processList)));
	if (!((longAt((void *)((processList + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))))) == GIV(nilObj))) {
		addLastLinktoList(activeProc, processList);

		/* transferToHighestPriorityProcessFrom: */
		transferTofrom(wakeHighestPriority(), CSYield);
	}
}