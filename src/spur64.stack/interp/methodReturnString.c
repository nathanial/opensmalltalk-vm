/* Extracted from interp.c:57851 (function methodReturnString). */

/*	Attempt to answer a ByteString for a given C string as the result of a
	primitive. 
 */

	/* StackInterpreter>>#methodReturnString: */

NeverInline sqInt
methodReturnString(const char *aCString)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt result;
    char *sp;

	assert(!((failed())));
	if (aCString) {
		if ((result = stringForCString(aCString))) {
			/* begin pop:thenPush: */
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),result);
			GIV(stackPointer) = sp;
		}
		else {
			GIV(primFailCode) = PrimErrNoMemory;
		}
	}
	else {
		GIV(primFailCode) = PrimErrOperationFailed;
	}
	return 0;
}