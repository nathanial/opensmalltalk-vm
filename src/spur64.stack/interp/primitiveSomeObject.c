/* Extracted from interp.c:24075 (function primitiveSomeObject). */

/*	Return the first object in the heap. */

	/* InterpreterPrimitives>>#primitiveSomeObject */

static void
primitiveSomeObject(void)
{
    sqInt object;
    char *sp;

	/* begin pop: */
	stackPointer += (argumentCount + 1) * BytesPerWord;
	object = firstAccessibleObject();

	/* begin push: */
	longAtput((sp = stackPointer - BytesPerWord),object);
	stackPointer = sp;
}