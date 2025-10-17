/* Extracted from interp.c:61182 (function pushBool). */

	/* StackInterpreter>>#pushBool: */

void
pushBool(sqInt trueOrFalse)
{
    sqInt object;
    char *sp;

	object = /* booleanObjectOf: */
			(trueOrFalse
				? trueObj
				: falseObj);

	/* begin push: */
	longAtput((sp = stackPointer - BytesPerWord),object);
	stackPointer = sp;
}