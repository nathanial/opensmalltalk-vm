/* Extracted from interp.c:13236 (function primitiveBytesLeft). */

static void
primitiveBytesLeft(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt aBool;
    sqInt chunk;
    sqInt freeChunk;
    sqInt i;
    sqInt integerVal;
    char *sp;

	if (!GIV(argumentCount)) {
		/* begin pop:thenPushInteger: */
		longAtput((sp = GIV(stackPointer)),((GIV(totalFreeOldSpace) << 3) | 1));
		GIV(stackPointer) = sp;
		return;
	}

	/* old behavior - just return the size of free memory */
	if (GIV(argumentCount) == 1) {
		if ((longAt(GIV(stackPointer))) == GIV(nilObj)) {
			/* begin sizeOfLargestFreeChunk */
			freeChunk = findLargestFreeChunk();
			if (!freeChunk) {
				for (i = 0x3F; i >= 1; i += -1) {
					if ((chunk = GIV(freeLists)[i])) {
						integerVal = bytesInBody(chunk);
						goto l1;
					}
				}
				integerVal = 0;
				goto l1;
			}
			integerVal = bytesInBody(freeChunk);
			/* end sizeOfLargestFreeChunk */
l1:

			/* begin pop:thenPushInteger: */
			longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),(((usqInt)integerVal << 3) | 1));
			GIV(stackPointer) = sp;
			return;
		}

		/* new behaviour -including or excluding swap space depending on aBool */

		/* begin booleanValueOf: */
		if ((longAt(GIV(stackPointer))) == GIV(trueObj)) {
			aBool = 1;
			goto l2;
		}
		if ((longAt(GIV(stackPointer))) == GIV(falseObj)) {
			aBool = 0;
			goto l2;
		}

		/* begin success: */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		aBool = null;
		/* end booleanValueOf: */
l2:
		if (!GIV(primFailCode)) {
			integerVal = ((((GIV(totalFreeOldSpace) + (GIV(scavengeThreshold) - GIV(freeStart))) - (interpreterAllocationReserveBytes())) < 0) ? 0 : ((GIV(totalFreeOldSpace) + (GIV(scavengeThreshold) - GIV(freeStart))) - (interpreterAllocationReserveBytes())));

			/* begin pop:thenPushInteger: */
			longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),(((usqInt)integerVal << 3) | 1));
			GIV(stackPointer) = sp;
			return;
		}
	}

	/* Spur behavior; if argument is nil answer size of largest free chunk in oldSpace. */

	/* begin primitiveFail */
	if (!GIV(primFailCode)) {
		GIV(primFailCode) = 1;
	}
	return;
}