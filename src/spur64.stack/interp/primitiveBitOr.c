/* Extracted from interp.c:12849 (function primitiveBitOr). */

/*	Note no short-cut for SmallIntegers. Either the inline interpreter
	bytecode or the JIT primitive will handle this case. */

	/* InterpreterPrimitives>>#primitiveBitOr */

static void
primitiveBitOr(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt integerArgumentValue;
    usqInt integerReceiverValue;
    sqInt oop;
    char *sp;
    sqInt value;

	/* begin positiveMachineIntegerValueOf: */
	if (((((longAt(stackPointer))) & 7) == 1)) {
		value = ((longAt(stackPointer)) >> 3);
		if (value < 0) {
			/* begin primitiveFail */
			if (!primFailCode) {
				primFailCode = 1;
			}
			integerArgumentValue = ((usqIntptr_t) null);
			goto l1;
		}
		integerArgumentValue = ((usqIntptr_t) value);
		goto l1;
	}

	/* don't inline the rare case */
	integerArgumentValue = positiveMachineIntegerValueOfObj(longAt(stackPointer));
	/* end positiveMachineIntegerValueOf: */
l1:
	oop = longAt(stackPointer + (1 * BytesPerWord));

	/* begin positiveMachineIntegerValueOf: */
	if ((((oop) & 7) == 1)) {
		value = (oop >> 3);
		if (value < 0) {
			/* begin primitiveFail */
			if (!primFailCode) {
				primFailCode = 1;
			}
			integerReceiverValue = ((usqIntptr_t) null);
			goto l2;
		}
		integerReceiverValue = ((usqIntptr_t) value);
		goto l2;
	}

	/* don't inline the rare case */
	integerReceiverValue = positiveMachineIntegerValueOfObj(oop);
	/* end positiveMachineIntegerValueOf: */
l2:
	if (!primFailCode) {
		oop = positive64BitIntegerFor(integerArgumentValue | integerReceiverValue);

		/* begin pop:thenPush: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),oop);
		stackPointer = sp;
	}
}