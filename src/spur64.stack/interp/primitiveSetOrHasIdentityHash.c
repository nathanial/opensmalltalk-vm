/* Extracted from interp.c:22293 (function primitiveSetOrHasIdentityHash). */

static void
primitiveSetOrHasIdentityHash(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt hasHash;
    sqInt hash;
    sqInt isReceiverAClass;
    sqInt lastArg;
    sqInt oldHash;
    char *sp;
    sqInt thisReceiver;

	if (!GIV(argumentCount)) {
		hasHash = ((!((longAt(GIV(stackPointer))) & (tagMask()))))
			 && ((long32At((void *)((longAt(GIV(stackPointer))) + 4))) & (identityHashHalfWordMask()));

		/* begin methodReturnBool: */
		assert(!((failed())));
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),/* booleanObjectOf: */
			(hasHash
				? GIV(trueObj)
				: GIV(falseObj)));
		GIV(stackPointer) = sp;
		return;
	}
	isReceiverAClass = 0;
	if (GIV(argumentCount) == 2) {
		lastArg = longAt(GIV(stackPointer));
		if ((((lastArg) & 7) == 1)) {
			hash = lastArg;
			thisReceiver = longAt(GIV(stackPointer) + (1 * BytesPerWord));
		}
		else {
			thisReceiver = longAt(GIV(stackPointer) + (2 * BytesPerWord));
			hash = longAt(GIV(stackPointer) + (1 * BytesPerWord));
			if (lastArg == GIV(trueObj)) {
				isReceiverAClass = 1;
			}
			else {
				if (!(lastArg == GIV(falseObj))) {
					/* primitiveFailFor: */
					GIV(primFailCode) = PrimErrBadArgument;
					return;
				}
			}
		}
	}
	else {
		thisReceiver = longAt(GIV(stackPointer) + (1 * BytesPerWord));
		hash = longAt(GIV(stackPointer));
	}

	/* anObject primitiveSetIdentityHashTo: hash */
	if (!(((((hash) & 7) == 1))
		 && ((/* if written naturally, clang warns: unsequenced modification and access to 'hash' */
			(hash = (hash >> 3)),
		(hash & (identityHashHalfWordMask())) == hash)))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	if (!((!(thisReceiver & (tagMask()))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = (thisReceiver == (longAt(GIV(stackPointer) + (GIV(argumentCount) * BytesPerWord)))
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
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),(((usqInt)oldHash << 3) | 1));
	GIV(stackPointer) = sp;
}