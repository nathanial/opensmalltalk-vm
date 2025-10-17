/* Extracted from interp.c:69454 (function primitiveSetGCSemaphore). */

/*	Primitive. Indicate the semaphore to be signalled for upon garbage
	collection 
 */

	/* StackInterpreterPrimitives>>#primitiveSetGCSemaphore */

EXPORT(void)
primitiveSetGCSemaphore(void)
{
    sqInt index;
    sqInt integerPointer;

	/* begin stackIntegerValue: */
	integerPointer = longAt(stackPointer);
	if ((((integerPointer) & 7) == 1)) {
		index = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		index = 0;
	}
	if (!primFailCode) {
		gcSemaphoreIndex = index;

		/* begin pop: */
		stackPointer += argumentCount * BytesPerWord;
	}
}