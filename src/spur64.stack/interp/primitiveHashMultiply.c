/* Extracted from interp.c:17582 (function primitiveHashMultiply). */

static void
primitiveHashMultiply(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt ccIndex;
    sqInt ok;
    char *sp;
    sqInt value;

	value = longAt(GIV(stackPointer));
	if ((((value) & 7) == 1)) {
		value = (value >> 3);
	}
	else {
		/* begin is:instanceOf:compactClassIndex: */
		if (((value & (tagMask())) != 0)) {
			ok = 0;
			goto l1;
		}

		/* begin isClassOfNonImm:equalTo:compactClassIndex: */
		assert(!(isImmediate(value)));
		ccIndex = (longAt((void *)(value))) & (classIndexMask());
		ok = ClassLargePositiveIntegerCompactIndex == ccIndex;
		/* end is:instanceOf:compactClassIndex: */
l1:
		if (!ok) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadReceiver;
			return;
		}
		value = SQ_SWAP_4_BYTES_IF_BIGENDIAN((long32At((void *)((value + BaseHeaderSize)))));
	}

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer)),(((((((usqInt) value)) * HashMultiplyConstant) & 0xFFFFFFF) << 3) | 1));
	GIV(stackPointer) = sp;
}