/* Extracted from interp.c:35143 (function findStringBeginningWith). */

/*	Print the oops of all string-like things that start with the same
	characters as aCString
 */
/*	useful for debugging */

	/* SpurMemoryManager>>#findStringBeginningWith: */

void
findStringBeginningWith(char *aCString)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt aCStringStrlen;
    sqInt address;
    sqInt classIndex;
    sqInt followingWord;
    usqInt followingWordAddress;
    usqInt numSlots;
    sqInt objSqInt;
    sqInt prevObj;
    sqInt prevPrevObj;
    sqInt startObject;

	aCStringStrlen = strlen(aCString);

	/* begin allObjectsDo: */
	address = /* startAddressForBridgedHeapEnumeration */
			(pastSpaceStart > (((pastSpace).start))
				? ((pastSpace).start)
				: (freeStart > (((eden).start))
						? ((eden).start)
						: oldSpaceStart));

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
		if (!(oopisLessThan(objSqInt, endOfMemory))) break;
		assert((long64At((void *)(objSqInt))) != 0);

		/* begin isEnumerableObject: */
		classIndex = (longAt((void *)(objSqInt))) & (classIndexMask());
		assert((classIndex == (segmentBridgePun()))
		 || ((classIndex == (isForwardedObjectClassIndexPun()))
		 || (((long64At((void *)(objSqInt))) != 0)
		 && (classIndex < (numClassTablePages * (classTablePageSize()))))));
		if (classIndex >= (isForwardedObjectClassIndexPun())) {
			if (/* object:beginsWithString:ofSize: */
				(/* isBytes: */
				((!(objSqInt & (tagMask()))))
			 && (((byteAt((void *)(objSqInt + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat())))
			 && ((!(((byteAt((void *)(objSqInt + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))
			 && (((numBytesOfBytes(objSqInt)) >= aCStringStrlen)
			 && ((strncmp(aCString, firstIndexableField(objSqInt), aCStringStrlen)) == 0)))) {
				printHex(objSqInt);

				/* begin space */
				printChar(' ');
				fprintf(transcript,
						"%" PRIdSQINT "",
						((sqInt)(lengthOf(objSqInt))));
				printChar(' ');
				printOopShortInner(objSqInt);
				cr();
			}
		}
		prevPrevObj = prevObj;
		prevObj = objSqInt;

		/* begin objectAfterMaybeSlimBridge:limit: */
		followingWordAddress = addressAfter(objSqInt);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
			objSqInt = endOfMemory;
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? ((oopisLessThan(objSqInt, oldSpaceStart))
					 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
							? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
							: followingWordAddress + BaseHeaderSize)
					: followingWordAddress);
		/* end objectAfterMaybeSlimBridge:limit: */
l1:
		assert(oopisGreaterThan(objSqInt, prevObj));
	}
}