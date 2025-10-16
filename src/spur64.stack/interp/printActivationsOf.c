/* Extracted from interp.c:42550 (function printActivationsOf). */

void
printActivationsOf(sqInt aMethodObj)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt address;
    sqInt classIndex;
    sqInt followingWord;
    usqInt followingWordAddress;
    usqInt numSlots;
    sqInt objSqInt;
    sqInt prevObj;
    sqInt prevPrevObj;
    sqInt startObject;

	/* begin allObjectsDo: */
	address = /* startAddressForBridgedHeapEnumeration */
			(GIV(pastSpaceStart) > (((GIV(pastSpace)).start))
				? ((GIV(pastSpace)).start)
				: (GIV(freeStart) > (((GIV(eden)).start))
						? ((GIV(eden)).start)
						: GIV(oldSpaceStart)));

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
	startObject = (numSlots == (numSlotsMask())
				? address + BaseHeaderSize
				: address);

	/* begin allEntitiesFrom:do: */
	prevPrevObj = (prevObj = null);
	objSqInt = startObject;
	enableObjectEnumerationFrom(startObject);
	while (1) {
		assert((objSqInt % (allocationUnit())) == 0);
		if (!(oopisLessThan(objSqInt, GIV(endOfMemory)))) break;
		assert((long64At((void *)(objSqInt))) != 0);

		/* begin isEnumerableObject: */
		classIndex = (longAt((void *)(objSqInt))) & (classIndexMask());
		assert((classIndex == (segmentBridgePun()))
		 || ((classIndex == (isForwardedObjectClassIndexPun()))
		 || (((long64At((void *)(objSqInt))) != 0)
		 && (classIndex < (GIV(numClassTablePages) * (classTablePageSize()))))));
		if (classIndex >= (isForwardedObjectClassIndexPun())) {
			if ((((longAt((void *)(objSqInt))) & (classIndexMask())) == ClassMethodContextCompactIndex)
			 && (aMethodObj == (longAt((void *)((objSqInt + BaseHeaderSize) + ((((usqInt)(MethodIndex) << (shiftForWord()))))))))) {
				printHex(objSqInt);

				/* begin space */
				printChar(' ');
				printOopShortInner(objSqInt);
				print(" pc ");
				printHex(longAt((void *)((objSqInt + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord())))))));
				cr();
			}
		}
		prevPrevObj = prevObj;
		prevObj = objSqInt;

		/* begin objectAfterMaybeSlimBridge:limit: */
		followingWordAddress = addressAfter(objSqInt);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
			objSqInt = GIV(endOfMemory);
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? ((oopisLessThan(objSqInt, GIV(oldSpaceStart)))
					 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
							? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
							: followingWordAddress + BaseHeaderSize)
					: followingWordAddress);
		/* end objectAfterMaybeSlimBridge:limit: */
l1:
		assert(oopisGreaterThan(objSqInt, prevObj));
	}
}