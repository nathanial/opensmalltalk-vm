/* Extracted from interp.c:60115 (function printLogEntryAt). */

static NoDbgRegParms void
printLogEntryAt(sqInt i)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt intOrClass;
    sqInt selectorMethodOrProcess;
    sqInt source;

	intOrClass = GIV(traceLog)[i];
	selectorMethodOrProcess = GIV(traceLog)[i + 1];
	source = GIV(traceLog)[i + 2];
	print(traceSources[source]);

	/* begin space */
	printChar(' ');
	if ((((intOrClass) & 7) == 1)) {
		switch (intOrClass) {
		case TraceStackOverflow:
			print("stack overflow");
			break;
		case TraceContextSwitch:
			print("context switch from ");
			printHex(selectorMethodOrProcess);
			break;
		case TraceIncrementalGC:
			print("incrementalGC");
			break;
		case TraceFullGC:
			print("fullGC");
			break;
		case TraceVMCallback:
			print("callback");
			break;
		case TraceVMCallbackReturn:
			print("return from callback");
			break;
		default:
			print("UNKNOWN EVENT");
		}
	}
	else {
		/* begin space */
		printChar(' ');
		printNameOfClasscount(intOrClass, 5);
		print(">>");
		printStringOf(selectorMethodOrProcess);
	}
	cr();
}