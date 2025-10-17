/* Extracted from interp.c:12711 (function primitiveBehaviorHash). */

/*	For the mirror prims check that the class obj is actually a valid class.
	No need to check if it's the receiver since the method exists only on
	Behavior. 
 */

	/* InterpreterPrimitives>>#primitiveBehaviorHash */

static void
primitiveBehaviorHash(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt err;
    sqInt hash;
    sqInt hashOrError;
    sqInt shouldBeClassObj;
    char *sp;

	shouldBeClassObj = longAt(GIV(stackPointer));
	if (GIV(argumentCount) > 0) {
		if ((((shouldBeClassObj & (tagMask())) != 0))
		 || (((!((longAt((void *)(shouldBeClassObj))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))
		 || (!(objCouldBeClassObj(shouldBeClassObj))))) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadArgument;
			return;
		}
	}
	assert((isNonImmediate(shouldBeClassObj))
	 && (addressCouldBeClassObj(shouldBeClassObj)));

	/* begin ensureBehaviorHash: */
	assert(addressCouldBeClassObj(shouldBeClassObj));

	/* eem 12/28/2021 the above asserft is too weak (and only an assert) */
	hashOrError = ((hash = (long32At((void *)(shouldBeClassObj + 4))) & (identityHashHalfWordMask()))
				? hash
				: (objCouldBeClassObj(shouldBeClassObj)
						? ((err = enterIntoClassTable(shouldBeClassObj))
								? -err
								: (long32At((void *)(shouldBeClassObj + 4))) & (identityHashHalfWordMask()))
						: -PrimErrBadReceiver));
	if (hashOrError < 0) {
		/* primitiveFailFor: */
		GIV(primFailCode) = -hashOrError;
		return;
	}

	/* begin methodReturnInteger: */
	assert(!((failed())));
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),(((usqInt)hashOrError << 3) | 1));
	GIV(stackPointer) = sp;
}