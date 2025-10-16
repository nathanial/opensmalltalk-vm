/* Extracted from interp.c:14697 (function primitiveCrashVM). */

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