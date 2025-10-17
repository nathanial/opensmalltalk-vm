/* Extracted from interp.c:14742 (function primitiveDisablePowerManager). */

/*	Pass in a non-negative value to disable the architectures powermanager if
	any, zero to enable. This is a named (not numbered) primitive in the null
	module (ie the VM)
 */

	/* InterpreterPrimitives>>#primitiveDisablePowerManager */

EXPORT(sqInt)
primitiveDisablePowerManager(void)
{
    sqInt integer;
    sqInt integerPointer;

	/* begin stackIntegerValue: */
	integerPointer = longAt(stackPointer);
	if ((((integerPointer) & 7) == 1)) {
		integer = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		integer = 0;
	}
	if (!primFailCode) {
		ioDisablePowerManager(integer);

		/* begin pop: */
		stackPointer += 1 * BytesPerWord;
	}
	return 0;
}