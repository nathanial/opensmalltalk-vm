/* Extracted from interp.c:55602 (function loadInitialContext). */

	/* StackInterpreter>>#loadInitialContext */

void
loadInitialContext(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt activeContext;
    sqInt activeProc;
    sqInt obj;
    sqInt objOop;

	runLeakCheckerFor(GCModeFull);

	/* primitiveSuspend needs to know the class of LinkedList */

	/* begin getClassTagOfLinkedList */
	objOop = longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));
	objOop = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(ProcessListsIndex) << (shiftForWord()))))));
	obj = longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
	GIV(classLinkedListClassTag) = (longAt((void *)(obj))) & (classIndexMask());
	objOop = longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));
	activeProc = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));
	activeContext = longAt((void *)((activeProc + BaseHeaderSize) + ((((usqInt)(SuspendedContextIndex) << (shiftForWord()))))));
	marryContextInNewStackPageAndInitializeInterpreterRegisters(activeContext);
}