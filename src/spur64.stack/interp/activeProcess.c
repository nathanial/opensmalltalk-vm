/* Extracted from interp.c:49882 (function activeProcess). */

sqInt
activeProcess(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt objOop;

	objOop = longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));

	/* begin fetchPointer:ofObject: */
	return longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));
}