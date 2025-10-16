/* Extracted from interp.c:25804 (function primitiveVMProfileSamplesInto). */

static void
primitiveVMProfileSamplesInto(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    long bufferSize;
    sqInt numSamples;
    sqInt running;
    sqInt sampleBuffer;
    char *sp;

	bufferSize = 0;
	running = 0;
	ioNewProfileStatus((&running), (&bufferSize));
	if (!GIV(argumentCount)) {
		/* begin pop:thenPushBool: */
		longAtput((sp = GIV(stackPointer)),/* booleanObjectOf: */
			(running
				? GIV(trueObj)
				: GIV(falseObj)));
		GIV(stackPointer) = sp;
		return;
	}
	if (!(GIV(argumentCount) == 1)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadNumArgs;
		return;
	}
	sampleBuffer = longAt(GIV(stackPointer));
	if (!(((!(sampleBuffer & (tagMask()))))
		 && ((isPureBitsNonImm(sampleBuffer))
		 && ((numBytesOf(sampleBuffer)) >= (bufferSize * BytesPerWord))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	numSamples = ioNewProfileSamplesInto(pointerForOop(sampleBuffer + BaseHeaderSize));

	/* begin pop:thenPushInteger: */
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),(((usqInt)numSamples << 3) | 1));
	GIV(stackPointer) = sp;
}