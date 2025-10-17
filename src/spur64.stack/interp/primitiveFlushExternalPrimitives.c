/* Extracted from interp.c:16385 (function primitiveFlushExternalPrimitives). */

/*	Primitive. Flush all the existing external primitives in the image thus
	forcing a reload on next invocation.
 */

	/* InterpreterPrimitives>>#primitiveFlushExternalPrimitives */

static void
primitiveFlushExternalPrimitives(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt address;
    sqInt classIndex;
    sqInt followingWord;
    usqInt followingWordAddress;
    usqInt numSlots;
    sqInt obj;
    sqInt prevObj;
    sqInt prevPrevObj;
    sqInt startObject;

	/* begin flushExternalPrimitives */
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
			if (((byteAt((void *)(obj + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())) {
				flushExternalPrimitiveOf(obj);
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

	/* begin flushMethodCache */
	memset(GIV(methodCache), 0, MethodCacheSize * (sizeof(GIV(methodCache)[0])));

	/* this for primitiveExternalMethod */
	GIV(lastMethodCacheProbeWrite) = 0;

	/* begin flushAtCache */
	memset(GIV(atCache), 0, AtCacheTotalSize * (sizeof(GIV(atCache)[0])));
	memset(externalPrimitiveTable, 0, MaxExternalPrimitiveTableSize * (sizeof(externalPrimitiveTable[0])));
	GIV(externalPrimitiveTableFirstFreeIndex) = 0;
}