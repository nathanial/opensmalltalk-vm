/* Extracted from interp.c:59560 (function printContextCallStackOf). */

static NoDbgRegParms sqInt
printContextCallStackOf(sqInt aContext)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt ctxt;

	ctxt = aContext;
	while (!((ctxt == GIV(nilObj))
	 || (((((longAt((void *)((ctxt + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1)))) {
		shortPrintContext(ctxt);
		ctxt = longAt((void *)((ctxt + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
	}
	return ctxt;
}