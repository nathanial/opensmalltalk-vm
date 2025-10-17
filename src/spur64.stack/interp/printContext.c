/* Extracted from interp.c:59578 (function printContext). */

/*	useful for VM debugging */

	/* StackInterpreter>>#printContext: */

void
printContext(sqInt aContext)
{
    sqInt i;
    sqInt ip;
    sqInt sender;
    sqInt sp;
    sqInt theMethod;

	shortPrintContext(aContext);
	sender = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
	ip = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))));
	if ((((sender) & 7) == 1)) {
		if (checkIsStillMarriedContextcurrentFP(aContext, framePointer)) {
			print("married (assuming framePointer valid)");
			cr();
		}
		else {
			print("widowed (assuming framePointer valid)");
			cr();
		}
		print("sender   ");
		printNum(sender);
		print(" (");
		printHexPtrnp(
			(/* begin withoutSmallIntegerTags: */
				assert((((sender) & 7) == 1)),
			((char *)(sender - (smallIntegerTag())))));
		printChar(')');
		cr();
		print("ip       ");
		printNum(ip);
		print(" (");
		printHexPtrnp(
			(/* begin withoutSmallIntegerTags: */
				assert((((ip) & 7) == 1)),
			((char *)(ip - (smallIntegerTag())))));
		printChar(')');
		cr();
	}
	else {
		print("sender   ");
		shortPrintOop(sender);
		print("ip       ");
		if (ip == nilObj) {
			shortPrintOop(ip);
		}
		else {
			printNum(ip);
			print(" (");
			printNum((ip >> 3));

			/* begin space */
			printChar(' ');
			printHex((ip >> 3));
			printChar(')');
			cr();
		}
	}
	sp = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
	print("sp       ");
	printNum((sp >> 3));
	print(" (");
	printHex(sp);
	printChar(')');
	cr();
	print("method   ");

	/* begin printMethodFieldForPrintContext: */
	theMethod = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(MethodIndex) << (shiftForWord()))))));
	fprintf(transcript,
			"%p: ",
			((void *)theMethod));
	shortPrintOop(theMethod);
	print("closure  ");
	shortPrintOop(longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(ClosureIndex) << (shiftForWord())))))));
	print("receiver ");
	shortPrintOop(longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(ReceiverIndex) << (shiftForWord())))))));
	sp = (sp >> 3);
	sp = ((sp < ((lengthOf(aContext)) - ReceiverIndex)) ? sp : ((lengthOf(aContext)) - ReceiverIndex));
	for (i = 1; i <= sp; i += 1) {
		print("       ");
		printNum(i);

		/* begin space */
		printChar(' ');
		shortPrintOop(longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)((ReceiverIndex + i)) << (shiftForWord())))))));
	}
}