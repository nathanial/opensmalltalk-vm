/* Extracted from interp.c:52235 (function ensureImageFormatIsUpToDate). */

/*	Ensure the image data has been updated to suit the current VM. */

	/* StackInterpreter>>#ensureImageFormatIsUpToDate: */

static NoDbgRegParms void
ensureImageFormatIsUpToDate(sqInt swapBytes)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt addr;
    sqInt address;
    sqInt classIndex;
    sqInt fmt;
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt methodHeader;
    usqInt numSlots;
    sqInt obj;
    sqInt objSqInt;
    sqInt prevObj;
    sqInt prevPrevObj;
    sqInt startObject;
    sqInt stopAddr;
    int swapFloatWords;
    sqInt temp;
    sqInt wordAddr;

	if (swapBytes) {
		/* begin reverseBytesInImage */
		/* begin reverseBytesInMemory */
		/* begin reverseBytesFrom:to: */
		addr = GIV(oldSpaceStart);
		while (oopisLessThan(addr, GIV(endOfMemory))) {
			longAtput((void *)(addr),SQ_SWAP_8_BYTES((longAt((void *)(addr)))));
			addr += BytesPerWord;
		}

		/* Second, return the bytes of bytes-type objects to their
		   orginal order, and perform any other format conversions. */

		/* begin updateObjectsPostByteSwap */
		swapFloatWords = VMBIGENDIAN != GIV(imageFloatsBigEndian);
		assert(ClassFloatCompactIndex != 0);

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
		obj = startObject;
		enableObjectEnumerationFrom(startObject);
		while (1) {
			assert((obj % (allocationUnit())) == 0);
			if (!(oopisLessThan(obj, GIV(endOfMemory)))) break;
			assert((long64At((void *)(obj))) != 0);

			/* begin isEnumerableObject: */
			classIndex = (longAt((void *)(obj))) & (classIndexMask());
			assert((classIndex == (segmentBridgePun()))
			 || ((classIndex == (isForwardedObjectClassIndexPun()))
			 || (((long64At((void *)(obj))) != 0)
			 && (classIndex < (GIV(numClassTablePages) * (classTablePageSize()))))));
			if (classIndex >= (isForwardedObjectClassIndexPun())) {
				fmt = (byteAt((void *)(obj + (formatFieldByteOffset())))) & (formatMask());
				if (fmt >= (firstByteFormat())) {
					wordAddr = obj + BaseHeaderSize;
					if (fmt >= (firstCompiledMethodFormat())) {
						methodHeader = longAt((void *)(obj + BaseHeaderSize));
						wordAddr += (((/* begin literalCountOfMethodHeader: */
	assert((((methodHeader) & 7) == 1)),
/* literalCountOfAlternateHeader: */
	((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask)) + LiteralStart) * BytesPerOop;
					}

					/* compiled method; start after methodHeader and literals */
					stopAddr = obj + (((((/* begin numSlotsOf: */
	assert((classIndexOf(obj)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(obj + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(obj - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) << (shiftForWord()))) + BaseHeaderSize);

					/* begin reverseBytesFrom:to: */
					addr = wordAddr;
					while (oopisLessThan(addr, stopAddr)) {
						longAtput((void *)(addr),SQ_SWAP_8_BYTES((longAt((void *)(addr)))));
						addr += BytesPerWord;
					}
				}

				/* oop contains bytes */
				if (fmt == (firstLongFormat())) {
					if (swapFloatWords
					 && (((longAt((void *)(obj))) & (classIndexMask())) == ClassFloatCompactIndex)) {
						temp = longAt((void *)(obj + BaseHeaderSize));
						longAtput((void *)(obj + BaseHeaderSize),longAt((void *)((obj + BaseHeaderSize) + 4)));
						longAtput((void *)((obj + BaseHeaderSize) + 4),temp);
					}
					else {
					}
				}
			}
			prevPrevObj = prevObj;
			prevObj = obj;

			/* begin objectAfterMaybeSlimBridge:limit: */
			followingWordAddress = addressAfter(obj);
			if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
				obj = GIV(endOfMemory);
				goto l1;
			}
			followingWord = longAt((void *)(followingWordAddress));
			obj = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
						? ((oopisLessThan(obj, GIV(oldSpaceStart)))
						 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
								? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
								: followingWordAddress + BaseHeaderSize)
						: followingWordAddress);
			/* end objectAfterMaybeSlimBridge:limit: */
l1:
			assert(oopisGreaterThan(obj, prevObj));
		}
	}
	else {
		/* begin convertFloatsToPlatformOrder */
		if (VMBIGENDIAN == GIV(imageFloatsBigEndian)) {
			goto l3;
		}
		assert(ClassFloatCompactIndex != 0);

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
				if (((longAt((void *)(objSqInt))) & (classIndexMask())) == ClassFloatCompactIndex) {
					temp = long32At((void *)(objSqInt + BaseHeaderSize));
					long32Atput((void *)(objSqInt + BaseHeaderSize),long32At((void *)((objSqInt + BaseHeaderSize) + 4)));
					long32Atput((void *)((objSqInt + BaseHeaderSize) + 4),temp);
				}
			}
			prevPrevObj = prevObj;
			prevObj = objSqInt;

			/* begin objectAfterMaybeSlimBridge:limit: */
			followingWordAddress = addressAfter(objSqInt);
			if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
				objSqInt = GIV(endOfMemory);
				goto l2;
			}
			followingWord = longAt((void *)(followingWordAddress));
			objSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
						? ((oopisLessThan(objSqInt, GIV(oldSpaceStart)))
						 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
								? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
								: followingWordAddress + BaseHeaderSize)
						: followingWordAddress);
			/* end objectAfterMaybeSlimBridge:limit: */
l2:
			assert(oopisGreaterThan(objSqInt, prevObj));
		}
		/* end convertFloatsToPlatformOrder */
l3:;
	}
}