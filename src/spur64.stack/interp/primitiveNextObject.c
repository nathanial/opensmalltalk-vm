/* Extracted from interp.c:19797 (function primitiveNextObject). */

/*	Return the object following the receiver in the heap. Return the
	SmallInteger zero when there are no more objects.
 */

	/* InterpreterPrimitives>>#primitiveNextObject */

static void
primitiveNextObject(void)
{
    sqInt instance;
    char *sp;

	if ((((longAt(stackPointer)) & (tagMask())) != 0)) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadReceiver;
		return;
	}
	if ((instance = accessibleObjectAfter(longAt(stackPointer)))) {
		assert(isInMemory(instance));

		/* begin pop:thenPush: */
		longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),instance);
		stackPointer = sp;
	}
	else {
		/* begin pop:thenPushInteger: */
		longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),(((usqInt)0 << 3) | 1));
		stackPointer = sp;
	}
}