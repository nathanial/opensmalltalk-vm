/* Extracted from interp.c:61182 (function pushBool). */

	/* StackInterpreter>>#pushBool: */

void
pushBool(sqInt trueOrFalse)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
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