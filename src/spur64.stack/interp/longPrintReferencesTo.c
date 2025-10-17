/* Extracted from interp.c:38888 (function longPrintReferencesTo). */

/*	Scan the heap long printing the oops of any and all objects that refer to
	anOop 
 */
/*	useful for VM debugging */

	/* SpurMemoryManager>>#longPrintReferencesTo: */

void
longPrintReferencesTo(sqInt anOop)
{
    sqInt address;
    sqInt classIndex;
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt i;
    usqInt numSlots;
    sqInt objSqInt;
    sqInt prevObj;
    sqInt prevPrevObj;
    sqInt prntObj;
    sqInt startObject;

	prntObj = 0;

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
			if ((((byteAt((void *)(objSqInt + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */)
			 || (((byteAt((void *)(objSqInt + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat()))) {
				if (((byteAt((void *)(objSqInt + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())) {
					i = (literalCountOf(objSqInt)) + LiteralStart;
				}
				else {
					if (((longAt((void *)(objSqInt))) & (classIndexMask())) == ClassMethodContextCompactIndex) {
						i = CtxtTempFrameStart + (fetchStackPointerOf(objSqInt));
					}
					else {
						/* begin numSlotsOf: */
						assert((classIndexOf(objSqInt)) > (isForwardedObjectClassIndexPun()));
						i = (((numSlots = byteAt((void *)(objSqInt + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
									? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objSqInt - BaseHeaderSize)))) << 8)))))) >> 8
									: numSlots);
					}
				}
				while (((i -= 1)) >= 0) {
					if (anOop == (longAt((void *)((objSqInt + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord())))))))) {
						fprintf(transcript,
								"%p @ %d\n",
								((void *)objSqInt),
								((int) i));
						prntObj = 1;
						i = 0;
					}
				}
				if (prntObj) {
					prntObj = 0;
					longPrintOop(objSqInt);
				}
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