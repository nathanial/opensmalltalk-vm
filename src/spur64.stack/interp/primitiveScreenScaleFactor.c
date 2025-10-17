/* Extracted from interp.c:21816 (function primitiveScreenScaleFactor). */

/*	Answer a float indicating the current scale factor for pixels of the
	Smalltalk window.
 */

	/* InterpreterPrimitives>>#primitiveScreenScaleFactor */

EXPORT(sqInt)
primitiveScreenScaleFactor(void)
{
    double factor;
    char *sp;

	factor = ioScreenScaleFactor();
	if (!primFailCode) {
		/* begin methodReturnFloat: */
		assert(!((failed())));
		longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),floatObjectOf(factor));
		stackPointer = sp;
	}
	return 0;
}