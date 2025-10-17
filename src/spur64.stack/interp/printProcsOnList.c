/* Extracted from interp.c:60838 (function printProcsOnList). */

/*	useful for VM debugging */

	/* StackInterpreter>>#printProcsOnList: */

sqInt
printProcsOnList(sqInt procList)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt firstProc;
    sqInt proc;

	proc = (firstProc = followFieldofObject(FirstLinkIndex, procList));
	while (!(proc == nilObj)) {
		printProcessStack(proc);
		proc = followFieldofObject(NextLinkIndex, proc);
		if (proc == firstProc) {
			warning("circular process list!!");
			return null;
		}
	}
	return 0;
}