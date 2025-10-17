/* Extracted from interp.c:59430 (function printCallStackFP). */

	/* StackInterpreter>>#printCallStackFP: */

static NoDbgRegParms sqInt
printCallStackFP(char *theFP)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt context;

	context = shortReversePrintFrameAndCallers(theFP);
	while (!(context == GIV(nilObj))) {
		if (((((longAt((void *)((context + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1)) {
			if (!(checkIsStillMarriedContextcurrentFP(context, GIV(framePointer)))) {
				shortPrintContext(context);
				return null;
			}
			context = shortReversePrintFrameAndCallers(frameOfMarriedContext(context));
		}
		else {
			context = printContextCallStackOf(context);
		}
	}
	return 0;
}