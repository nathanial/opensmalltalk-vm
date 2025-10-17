/* Extracted from interp.c:60028 (function printLikelyImplementorsOfSelector). */

/*	Print all methods whose penultimate literal is either selector,
	or an object whose first inst var is the method and whose
	second is selector (e.g. an AdditionalMethodState). */
/*	useful for VM debugging */

	/* StackInterpreter>>#printLikelyImplementorsOfSelector: */

void
printLikelyImplementorsOfSelector(sqInt selector)
{
    sqInt address;
    sqInt classIndex;
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt methodClassAssociation;
    usqInt numSlots;
    sqInt objSqInt;
    sqInt prevObj;
    sqInt prevPrevObj;
    sqInt startObject;

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
			if ((((byteAt((void *)(objSqInt + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat()))
			 && ((maybeSelectorOfMethod(objSqInt)) == selector)) {
				methodClassAssociation = methodClassAssociationOf(objSqInt);

				/* printHexnp: */
				fprintf(transcript,
						"%p",
						((void *)objSqInt));

				/* begin space */
				printChar(' ');
				printOopShortInner(((/* isPointers: */
					((!(methodClassAssociation & (tagMask()))))
				 && (((byteAt((void *)(methodClassAssociation + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */))
				 && ((numSlotsOf(methodClassAssociation)) >= 2)
						? longAt((void *)((methodClassAssociation + BaseHeaderSize) + (0U << (shiftForWord()))))
						: methodClassAssociation));
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