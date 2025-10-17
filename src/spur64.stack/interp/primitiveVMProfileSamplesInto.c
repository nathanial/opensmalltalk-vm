/* Extracted from interp.c:25804 (function primitiveVMProfileSamplesInto). */

/*	Primitive.
	0 args: Answer whether the VM Profiler is running or not.
	1 arg:	Copy the sample data into the supplied argument, which must be a
	Bitmap of suitable size. Answer the number of samples copied into the
	buffer.  */

	/* InterpreterPrimitives>>#primitiveVMProfileSamplesInto */

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
	if (!argumentCount) {
		/* begin pop:thenPushBool: */
		longAtput((sp = stackPointer),/* booleanObjectOf: */
			(running
				? trueObj
				: falseObj));
		stackPointer = sp;
		return;
	}
	if (!(argumentCount == 1)) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadNumArgs;
		return;
	}
	sampleBuffer = longAt(stackPointer);
	if (!(((!(sampleBuffer & (tagMask()))))
		 && ((isPureBitsNonImm(sampleBuffer))
		 && ((numBytesOf(sampleBuffer)) >= (bufferSize * BytesPerWord))))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		return;
	}
	numSamples = ioNewProfileSamplesInto(pointerForOop(sampleBuffer + BaseHeaderSize));

	/* begin pop:thenPushInteger: */
	longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),(((usqInt)numSamples << 3) | 1));
	stackPointer = sp;
}