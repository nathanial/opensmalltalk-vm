/* Extracted from interp.c:49882 (function activeProcess). */

/*	Answer the current activeProcess. */
/*	useful for VM debugging */

	/* StackInterpreter>>#activeProcess */

sqInt
activeProcess(void)
{
    sqInt objOop;

	objOop = longAt((void *)(((longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));

	/* begin fetchPointer:ofObject: */
	return longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));
}