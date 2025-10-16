/* Extracted from interp.c:44486 (function printReferencesTo). */

void
printReferencesTo(sqInt anOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt address;
    sqInt classIndex;
    sqInt contextSize;
    sqInt fmt;
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt header;
    sqInt i;
    usqInt numLiterals;
    usqInt numSlots;
    sqInt objSqInt;
    sqInt prevObj;
    sqInt prevPrevObj;
    sqInt sp;
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
			/* begin numPointerSlotsOf: */
			fmt = (byteAt((void *)(objSqInt + (formatFieldByteOffset())))) & (formatMask());
			if (fmt <= 5 /* lastPointerFormat */) {
				if ((fmt == (indexablePointersFormat()))
				 && (((longAt((void *)(objSqInt))) & (classIndexMask())) == ClassMethodContextCompactIndex)) {
					/* begin fetchStackPointerOf: */
					sp = longAt((void *)((objSqInt + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
					if (!((((sp) & 7) == 1))) {
						contextSize = 0;
						goto l2;
					}
					assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(objSqInt)));
					contextSize = (sp >> 3);
					/* end fetchStackPointerOf: */
l2:
					i = ((usqInt) (CtxtTempFrameStart + contextSize));
					goto l3;
				}

				/* contexts end at the stack pointer */

				/* begin numSlotsOf: */
				assert((classIndexOf(objSqInt)) > (isForwardedObjectClassIndexPun()));
				i = (((numSlots = byteAt((void *)(objSqInt + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
							? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objSqInt - BaseHeaderSize)))) << 8)))))) >> 8
							: numSlots);
				goto l3;
			}
			if (fmt == (forwardedFormat())) {
				i = 1;
				goto l3;
			}
			if (fmt < (firstCompiledMethodFormat())) {
				i = 0;
				goto l3;
			}

			/* begin methodHeaderOf: */
			assert(isCompiledMethod(objSqInt));
			header = longAt((void *)((objSqInt + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

			/* begin literalCountOfMethodHeader: */
			assert((((header) & 7) == 1));
			numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
			i = numLiterals + LiteralStart;
			/* end numPointerSlotsOf: */
l3:
			while (((i -= 1)) >= 0) {
				if (anOop == (longAt((void *)((objSqInt + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord())))))))) {
					printHex(objSqInt);
					print(" @ ");
					printNum(i);

					/* begin space */
					printChar(' ');
					printOopShortInner(objSqInt);
					cr();
					i = 0;
				}
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
	if (isonObjStack(anOop, GIV(markStack))) {
		print("on markStack");
		cr();
	}
	if (isonObjStack(anOop, GIV(weaklingStack))) {
		print("on weaklingStack");
		cr();
	}
	if (isonObjStack(anOop, GIV(mournQueue))) {
		print("in mournQueue");
		cr();
	}
	if ((((byteAt((void *)(anOop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift()))) != 0)
	 && (isInRememberedSet(anOop))) {
		print("in rememberedSet");
		cr();
	}
}