/* Extracted from interp.c:22293 (function primitiveSetOrHasIdentityHash). */

	/* InterpreterPrimitives>>#primitiveSetOrHasIdentityHash */

static void
primitiveSetOrHasIdentityHash(void)
{
    sqInt hasHash;
    sqInt hash;
    sqInt isReceiverAClass;
    sqInt lastArg;
    sqInt oldHash;
    char *sp;
    sqInt thisReceiver;

	if (!argumentCount) {
		hasHash = ((!((longAt(stackPointer)) & (tagMask()))))
			 && ((long32At((void *)((longAt(stackPointer)) + 4))) & (identityHashHalfWordMask()));

		/* begin methodReturnBool: */
		assert(!((failed())));
		longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),/* booleanObjectOf: */
			(hasHash
				? trueObj
				: falseObj));
		stackPointer = sp;
		return;
	}
	isReceiverAClass = 0;
	if (argumentCount == 2) {
		lastArg = longAt(stackPointer);
		if ((((lastArg) & 7) == 1)) {
			hash = lastArg;
			thisReceiver = longAt(stackPointer + (1 * BytesPerWord));
		}
		else {
			thisReceiver = longAt(stackPointer + (2 * BytesPerWord));
			hash = longAt(stackPointer + (1 * BytesPerWord));
			if (lastArg == trueObj) {
				isReceiverAClass = 1;
			}
			else {
				if (!(lastArg == falseObj)) {
					/* primitiveFailFor: */
					primFailCode = PrimErrBadArgument;
					return;
				}
			}
		}
	}
	else {
		thisReceiver = longAt(stackPointer + (1 * BytesPerWord));
		hash = longAt(stackPointer);
	}

	/* anObject primitiveSetIdentityHashTo: hash */
	if (!(((((hash) & 7) == 1))
		 && ((/* if written naturally, clang warns: unsequenced modification and access to 'hash' */
			(hash = (hash >> 3)),
		(hash & (identityHashHalfWordMask())) == hash)))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		return;
	}
	if (!((!(thisReceiver & (tagMask()))))) {
		/* primitiveFailFor: */
		primFailCode = (thisReceiver == (longAt(stackPointer + (argumentCount * BytesPerWord)))
					? PrimErrBadReceiver
					: PrimErrBadArgument);
		return;
	}
	oldHash = (long32At((void *)(thisReceiver + 4))) & (identityHashHalfWordMask());

	/* begin setHashBitsOf:to: */
	long32Atput((void *)(thisReceiver + 4),((((long32At((void *)(thisReceiver + 4))) | (identityHashHalfWordMask())) - (identityHashHalfWordMask()))) + (hash & (identityHashHalfWordMask())));
	if (isReceiverAClass) {
		classAtIndexput(hash, thisReceiver);

		/* this figures out if the index is ambiguous and fixes all the instances if needed */
		allInstancesOf(thisReceiver);
	}

	/* begin methodReturnInteger: */
	assert(!((failed())));
	longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),(((usqInt)oldHash << 3) | 1));
	stackPointer = sp;
}