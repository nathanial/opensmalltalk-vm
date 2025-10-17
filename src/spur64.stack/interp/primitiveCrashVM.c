/* Extracted from interp.c:14697 (function primitiveCrashVM). */

/*	Crash the VM by indirecting through a null pointer. If the sole argument
	is true crash in this thread, and if it is false crash in a new thread. If
	the argument is an integer use the method that implies.
	bit 0 = thread to crash in; 1 => this thread
	bit 1 = crash method; 0 => indirect through null pointer; 1 => call exit */

	/* InterpreterPrimitives>>#primitiveCrashVM */

EXPORT(sqInt)
primitiveCrashVM(void)
{
    sqInt crashInThisThread;

	if (((((longAt(stackPointer))) & 7) == 1)) {
		crashInThisThread = ((longAt(stackPointer)) >> 3);
	}
	else {
		/* begin booleanValueOf: */
		if ((longAt(stackPointer)) == trueObj) {
			crashInThisThread = 1;
			goto l1;
		}
		if ((longAt(stackPointer)) == falseObj) {
			crashInThisThread = 0;
			goto l1;
		}

		/* begin success: */
		if (!primFailCode) {
			primFailCode = 1;
		}
		crashInThisThread = null;
		/* end booleanValueOf: */
l1:;
	}
	if (primFailCode
	 || (argumentCount != 1)) {
		return (primFailCode = PrimErrBadNumArgs);
	}
	crashInThisOrAnotherThread(crashInThisThread);

	/* begin pop: */
	stackPointer += 1 * BytesPerWord;
	return 0;
}