/* Extracted from interp.c:21789 (function primitiveScreenDepth). */

/*	Return a SmallInteger indicating the current depth of the OS screen.
	Negative values are used to imply LSB type pixel format an there is some
	support in the VM for handling either MSB or LSB
 */

	/* InterpreterPrimitives>>#primitiveScreenDepth */

EXPORT(sqInt)
primitiveScreenDepth(void)
{
    sqInt depth;
    char *sp;

	depth = ioScreenDepth();
	if (primFailCode) {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		return null;
	}

	/* begin pop:thenPushInteger: */
	longAtput((sp = stackPointer),(((usqInt)depth << 3) | 1));
	stackPointer = sp;
	return 0;
}