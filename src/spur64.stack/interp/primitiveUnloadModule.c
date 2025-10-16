/* Extracted from interp.c:70634 (function primitiveUnloadModule). */

static void
primitiveUnloadModule(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt address;
    sqInt classIndex;
    sqInt fmt;
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt moduleLength;
    sqInt moduleName;
    usqInt numSlots;
    sqInt obj;
    sqInt prevObj;
    sqInt prevPrevObj;
    sqInt startObject;

	moduleName = longAt(GIV(stackPointer));
	if (!(/* isBytes: */
			((!(moduleName & (tagMask()))))
		 && (((byteAt((void *)(moduleName + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat())))) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return;
	}

	/* begin numBytesOfBytes: */
	fmt = (byteAt((void *)(moduleName + (formatFieldByteOffset())))) & (formatMask());
	assert(fmt >= (firstByteFormat()));
	moduleLength = ((((/* begin numSlotsOf: */
	assert((classIndexOf(moduleName)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(moduleName + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(moduleName - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) << (shiftForWord()))) - (fmt & 7);
	if (!(ioUnloadModuleOfLength(oopForPointer(firstIndexableField(moduleName)), moduleLength))) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return;
	}
	if (/* object:equalsString:ofSize: */
		(/* isBytes: */
		((!(moduleName & (tagMask()))))
	 && (((byteAt((void *)(moduleName + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat())))
	 && ((!(((byteAt((void *)(moduleName + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))
	 && (((numBytesOfBytes(moduleName)) == moduleLength)
	 && ((strncmp("SqueakFFIPrims", firstIndexableField(moduleName), moduleLength)) == 0)))) {
		primitiveCalloutPointer = ((void *) -1);
	}
	forceInterruptCheck();

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

	/* begin pop: */
	GIV(stackPointer) += 1 * BytesPerWord;
}