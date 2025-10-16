/* Extracted from interp.c:59416 (function printCallStack). */

void
printCallStack(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *fp;

	if ((fp = GIV(framePointer))) {
		printCallStackFP(fp);
	}
	else {
		printCallStackOf(fetchPointerofObject(SuspendedContextIndex, activeProcess()));
	}
}