/* Extracted from interp.c:19686 (function primitiveNextInstance). */

	/* InterpreterPrimitives>>#primitiveNextInstance */

static void
primitiveNextInstance(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt actualObj;
    sqInt classIndex;
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt object;
    char *sp;
    sqInt subsequentObject;

	object = longAt(GIV(stackPointer));
	if (!(((object & (tagMask())) != 0))) {
		/* begin instanceAfter: */
		actualObj = object;
		classIndex = (longAt((void *)(object))) & (classIndexMask());

		/* begin isYoungObject: */
		assert(isNonImmediate(object));
		if (oopisLessThan(object, GIV(oldSpaceStart))) {
			if (oopisGreaterThanOrEqualToandLessThan(object, ((GIV(eden)).start), GIV(freeStart))) {
				while (1) {
					/* begin objectAfter:limit: */
					followingWordAddress = addressAfter(actualObj);
					if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(freeStart))) {
						actualObj = GIV(freeStart);
						goto l1;
					}
					followingWord = longAt((void *)(followingWordAddress));
					actualObj = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
								? followingWordAddress + BaseHeaderSize
								: followingWordAddress);
					/* end objectAfter:limit: */
l1:;
					if (!(oopisLessThan(actualObj, GIV(freeStart)))) break;
					if (classIndex == ((longAt((void *)(actualObj))) & (classIndexMask()))) {
						subsequentObject = actualObj;
						goto l4;
					}
				}
				actualObj = (oopisGreaterThan(GIV(pastSpaceStart), ((GIV(pastSpace)).start))
							? objectStartingAt(((GIV(pastSpace)).start))
							: GIV(nilObj));
			}
			if (oopisGreaterThanOrEqualToandLessThan(actualObj, ((GIV(pastSpace)).start), GIV(pastSpaceStart))) {
				while (1) {
					/* begin objectAfter:limit: */
					followingWordAddress = addressAfter(actualObj);
					if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(pastSpaceStart))) {
						actualObj = GIV(pastSpaceStart);
						goto l2;
					}
					followingWord = longAt((void *)(followingWordAddress));
					actualObj = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
								? followingWordAddress + BaseHeaderSize
								: followingWordAddress);
					/* end objectAfter:limit: */
l2:;
					if (!(oopisLessThan(actualObj, GIV(pastSpaceStart)))) break;
					if (classIndex == ((longAt((void *)(actualObj))) & (classIndexMask()))) {
						subsequentObject = actualObj;
						goto l4;
					}
				}
				actualObj = GIV(nilObj);
			}
		}

		/* it almost never is because usually this is preceded by a send of initialInstanceOf: */
		while (1) {
			/* begin objectAfter:limit: */
			followingWordAddress = addressAfter(actualObj);
			if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
				actualObj = GIV(endOfMemory);
				goto l3;
			}
			followingWord = longAt((void *)(followingWordAddress));
			actualObj = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
						? followingWordAddress + BaseHeaderSize
						: followingWordAddress);
			/* end objectAfter:limit: */
l3:;
			if (!(oopisLessThan(actualObj, GIV(endOfMemory)))) break;
			if (classIndex == ((longAt((void *)(actualObj))) & (classIndexMask()))) {
				subsequentObject = actualObj;
				goto l4;
			}
		}
		subsequentObject = null;
		/* end instanceAfter: */
l4:
		if (subsequentObject) {
			/* begin pop:thenPush: */
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),subsequentObject);
			GIV(stackPointer) = sp;
			return;
		}
	}

	/* begin primitiveFail */
	if (!GIV(primFailCode)) {
		GIV(primFailCode) = 1;
	}
}