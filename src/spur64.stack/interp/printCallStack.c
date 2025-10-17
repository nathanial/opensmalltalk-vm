/* Extracted from interp.c:59416 (function printCallStack). */

/*	useful for VM debugging */

	/* StackInterpreter>>#printCallStack */

void
printCallStack(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *fp;

	if ((fp = framePointer)) {
		printCallStackFP(fp);
	}
	else {
		printCallStackOf(fetchPointerofObject(SuspendedContextIndex, activeProcess()));
	}
}