/* Extracted from interp.c:63119 (function shortPrintContext). */

	/* StackInterpreter>>#shortPrintContext: */

static NoDbgRegParms sqInt
shortPrintContext(sqInt aContext)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt home;
    sqInt rcvr;
    sqInt senderOop;
    char *theFP;

	if (!(/* isContext: */
			((!(aContext & (tagMask()))))
		 && (((longAt((void *)(aContext))) & (classIndexMask())) == ClassMethodContextCompactIndex))) {
		printHex(aContext);

		/* begin print: */
		fprintf(GIV(transcript),
				"%s",
				" is not a context");
		cr();
		return null;
	}
	printHex(aContext);
	if (((((longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1)) {
		if (checkIsStillMarriedContextcurrentFP(aContext, GIV(framePointer))) {
			/* begin frameOfMarriedContext: */
			senderOop = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
			assert((((senderOop) & 7) == 1));
			theFP = ((char *)(senderOop - (smallIntegerTag())));

			/* begin print: */
			fprintf(GIV(transcript),
					"%s",
					" I (");
			printHex(((usqIntptr_t)theFP));

			/* begin print: */
			fprintf(GIV(transcript),
					"%s",
					") ");
		}
		else {
			/* begin print: */
			fprintf(GIV(transcript),
					"%s",
					" w ");
		}
	}
	else {
		/* begin print: */
		fprintf(GIV(transcript),
				"%s",
				" s ");
	}
	if ((home = findHomeForContext(aContext))) {
		rcvr = longAt((void *)((home + BaseHeaderSize) + ((((usqInt)(ReceiverIndex) << (shiftForWord()))))));
		printActivationNameForreceiverisBlockfirstTemporary(longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(MethodIndex) << (shiftForWord())))))), rcvr, home != aContext, longAt((void *)((home + BaseHeaderSize) + ((((usqInt)((0 + CtxtTempFrameStart)) << (shiftForWord())))))));

		/* begin space */
		printChar(' ');
		shortPrintOop(rcvr);
	}
	else {
		/* begin print: */
		fprintf(GIV(transcript),
				"%s",
				" BOGUS CONTEXT (can't determine home)");
		cr();
	}
	return 0;
}