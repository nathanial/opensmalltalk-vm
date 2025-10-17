/* Extracted from interp.c:14697 (function primitiveCrashVM). */

/*	Crash the VM by indirecting through a null pointer. If the sole argument
	is true crash in this thread, and if it is false crash in a new thread. If
	the argument is an integer use the method that implies.
	bit 0 = thread to crash in; 1 => this thread
	bit 1 = crash method; 0 => indirect through null pointer; 1 => call exit */

	/* InterpreterPrimitives>>#primitiveCrashVM */

EXPORT(sqInt)
primitiveCrashVM(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt crashInThisThread;

	if (((((longAt(GIV(stackPointer)))) & 7) == 1)) {
		crashInThisThread = ((longAt(GIV(stackPointer))) >> 3);
	}
	else {
		/* begin booleanValueOf: */
		if ((longAt(GIV(stackPointer))) == GIV(trueObj)) {
			crashInThisThread = 1;
			goto l1;
		}
		if ((longAt(GIV(stackPointer))) == GIV(falseObj)) {
			crashInThisThread = 0;
			goto l1;
		}

		/* begin success: */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		crashInThisThread = null;
		/* end booleanValueOf: */
l1:;
	}
	if (GIV(primFailCode)
	 || (GIV(argumentCount) != 1)) {
		return (GIV(primFailCode) = PrimErrBadNumArgs);
	}
	crashInThisOrAnotherThread(crashInThisThread);

	/* begin pop: */
	GIV(stackPointer) += 1 * BytesPerWord;
	return 0;
}