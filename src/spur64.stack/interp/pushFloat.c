/* Extracted from interp.c:61211 (function pushFloat). */

	/* StackInterpreter>>#pushFloat: */

void
pushFloat(double f)
{
    sqInt object;
    char *sp;

	object = floatObjectOf(f);

	/* begin push: */
	longAtput((sp = stackPointer - BytesPerWord),object);
	stackPointer = sp;
}