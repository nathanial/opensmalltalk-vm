/* Extracted from interp.c:34218 (function doShortentoIndexableSize). */

/*	Reduce the number of indexable fields in objOop, a pointer object, to
	nSlots. If in oldSpace,
	convert the unused residual to a free chunk. If in eden, cut back
	freeStart if possible.
	Without changes to numSlotsForShortening:toIndexableSize: this only works
	for arrayFormat, longFormat, and on 64-bits sixtyFourBitIndexableFormat,
	objects. Answer the number of bytes returned to free memory, which may be
	zero if no change
	was possible. */

	/* SpurMemoryManager>>#doShorten:toIndexableSize: */

static NoDbgRegParms sqInt
doShortentoIndexableSize(sqInt objOop, sqInt indexableSize)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt bytesBefore;
    sqInt classIndex;
    sqInt copy;
    sqInt delta;
    usqInt followingAddress;
    sqInt format;
    sqInt formatSqInt;
    sqInt freeChunk;
    sqInt i;
    sqInt instFormat;
    usqInt newObj;
    usqInt numBytes;
    sqInt numSlots;
    sqInt valuePointer;

	/* for assert checking */
	followingAddress = addressAfter(objOop);
	assert(oopisLessThanOrEqualTo(followingAddress, GIV(endOfMemory)));

	/* numSlotsForShortening:toIndexableSize: */
	switch ((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask())) {
	case arrayFormat():
	case sixtyFourBitIndexableFormat():
		numSlots = indexableSize;
		break;
	case firstLongFormat():
	case (firstLongFormat()) + 1:
		numSlots = ((indexableSize * 4) + (BytesPerWord - 1)) / BytesPerWord;
		break;
	default:
		assertf("invalid format for shortening");
		numSlots = 0;
	}
	bytesBefore = bytesInBody(objOop);
	delta = bytesBefore - (/* objectBytesForSlots: */
	(numSlots
		? ((((usqInt)(numSlots) << (shiftForWord())))) + ((numSlots >= (numSlotsMask())
		? BaseHeaderSize + BaseHeaderSize
		: BaseHeaderSize))
		: 8 /* allocationUnit */ + BaseHeaderSize));

	/* Since the system rounds objects up to 64-bits, losing a 32-bit
	   slot may not actually change the bytes occupied by the object. */
	if (!delta) {
		if ((byteAt((void *)(objOop + (numSlotsFieldByteOffset())))) == (numSlotsMask())) {
			/* begin rawOverflowSlotsOf:put: */
			longAtput((void *)(objOop - BaseHeaderSize),((((usqInt)((numSlotsMask())) << 56))) + numSlots);
		}
		else {
			/* rawNumSlotsOf:put: */
			byteAtput((void *)(objOop + (numSlotsFieldByteOffset())),numSlots);
		}

		/* begin updateFormatOfShortenedObject:to: */
		if (((((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask())) >= (firstLongFormat())) && (((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask())) <= ((firstLongFormat()) + 1)))) {
			formatSqInt = (firstLongFormat()) + (indexableSize & 1);

			/* begin setFormatOf:to: */
			assert(((formatSqInt >= 0) && (formatSqInt <= (formatMask()))));
			byteAtput((void *)(objOop + (formatFieldByteOffset())),((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (0xFF - (formatMask()))) + formatSqInt);
		}
		return 0;
	}

	/* Currently we can't have one word gaps in oldSpace but we can in newSpace.  So only create
	   a copy and forward if objOop is in old space. If in newSpace we'll create a slimbridge below. */
	if ((delta <= 8 /* allocationUnit */)
	 && ((/* begin isOldObject: */
		assert(isNonImmediate(objOop)),
	oopisGreaterThanOrEqualTo(objOop, GIV(oldSpaceStart))))) {
		/* begin normalisedFormatFor:indexableSize: */
		instFormat = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
		if (instFormat >= (firstByteFormat())) {
			formatSqInt = instFormat & -8;
			goto l1;
		}

		/* this is likely the common case */
		if (instFormat <= (sixtyFourBitIndexableFormat())) {
			formatSqInt = instFormat;
			goto l1;
		}
		if (instFormat < (firstShortFormat())) {
			formatSqInt = instFormat & -2;
			goto l1;
		}
		formatSqInt = instFormat & -4;
		/* end classFormatFromInstFormat: */
l1:
		format = ((formatSqInt == (firstLongFormat()))
			 && (((indexableSize & 1) != 0))
					? formatSqInt + 1
					: formatSqInt);
		classIndex = (longAt((void *)(objOop))) & (classIndexMask());

		/* begin allocateSlots:format:classIndex: */
		if (numSlots >= (numSlotsMask())) {
			if ((((usqInt)(numSlots)) >> 56) > 0) {
				copy = null;
				goto l2;
			}
			newObj = GIV(freeStart) + BaseHeaderSize;
			numBytes = (BaseHeaderSize + BaseHeaderSize) + (numSlots * BytesPerOop);
		}
		else {
			newObj = GIV(freeStart);
			numBytes = BaseHeaderSize + ((numSlots < 1
		? 8 /* allocationUnit */
		: numSlots * BytesPerOop));
		}
		if ((GIV(freeStart) + numBytes) > GIV(scavengeThreshold)) {
			if (numSlots <= ((1U << (fixedFieldsFieldWidth())) - 1)) {
				if (!GIV(needGCFlag)) {
					/* begin scheduleScavenge */
					GIV(needGCFlag) = 1;
					forceInterruptCheck();
				}
			}
			copy = allocateSlotsInOldSpacebytesformatclassIndex(numSlots, numBytes, format, classIndex);
			goto l2;
		}
		if (numSlots >= (numSlotsMask())) {
			longAtput((void *)(GIV(freeStart)),((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) + numSlots);
			longAtput((void *)(newObj),((((((usqLong) (numSlotsMask()))) << (numSlotsFullShift()))) + ((((usqInt)(format) << (formatShift()))))) + classIndex);
		}
		else {
			longAtput((void *)(newObj),((((((usqLong) numSlots)) << (numSlotsFullShift()))) + ((((usqInt)(format) << (formatShift()))))) + classIndex);
		}

		/* for header parsing we put a saturated slot count in the prepended overflow size word */
		assert((numBytes % (allocationUnit())) == 0);
		assert((newObj % (allocationUnit())) == 0);
		GIV(freeStart) += numBytes;
		copy = newObj;
		/* end allocateSlots:format:classIndex: */
l2:
		if (!copy) {
			error("shorten:toIndexableSize: attempted to shorten by allocationUnit and failed to allocate space for copy!");
		}
		if (/* isPureBitsFormat: */
			(format >= (sixtyFourBitIndexableFormat()))
		 && (format < (firstCompiledMethodFormat()))) {
			memcpy(firstIndexableField(copy), firstIndexableField(objOop), numBytesOfBitsformat(copy, format));
		}
		else {
			for (i = 0; i < numSlots; i += 1) {
				valuePointer = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));

				/* begin storePointerUnchecked:ofObject:withValue: */
				assert((isNonImmediate(copy))
				 && (!(isForwarded(copy))));
				assert(validStorePointerUncheckedArgs(i, copy, valuePointer));
				longAtput((void *)((copy + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),valuePointer);
			}
		}
		if ((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift()))) {
			remember(copy);
		}

		/* begin forward:to: */
		/* begin set:classIndexTo:formatTo: */
		assert((((isForwardedObjectClassIndexPun()) >= 0) && ((isForwardedObjectClassIndexPun()) <= (classIndexMask()))));
		assert((((forwardedFormat()) >= 0) && ((forwardedFormat()) <= (formatMask()))));
		longAtput((void *)(objOop),((longAt((void *)(objOop))) & (~(usqIntptr_t)(((((usqInt)((formatMask())) << (formatShift())))) + (classIndexMask())))) + ((isForwardedObjectClassIndexPun()) + ((((usqInt)((forwardedFormat())) << (formatShift()))))));
#    if IMMUTABILITY
		/* begin setIsImmutableOf:to: */
		longAtput((void *)(objOop),(longAt((void *)(objOop))) & (~(usqIntptr_t)(1U << (immutableBitShift()))));
#    endif // IMMUTABILITY

		assert(isForwarded(objOop));
		assert(!(isOopForwarded(copy)));

		/* begin isOldObject: */
		assert(isNonImmediate(objOop));
		if (oopisGreaterThanOrEqualTo(objOop, GIV(oldSpaceStart))) {
			if (/* isYoung: */
				((!(copy & (tagMask()))))
			 && (oopisLessThan(copy, GIV(oldSpaceStart)))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(objOop);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))),copy);

		/* For safety make sure the forwarder has a slot count that includes its contents. */
		if (!(byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) {
			/* rawNumSlotsOf:put: */
			byteAtput((void *)(objOop + (numSlotsFieldByteOffset())),1);
		}
		return 0;
	}
	if ((byteAt((void *)(objOop + (numSlotsFieldByteOffset())))) == (numSlotsMask())) {
		/* begin rawOverflowSlotsOf:put: */
		longAtput((void *)(objOop - BaseHeaderSize),((((usqInt)((numSlotsMask())) << 56))) + numSlots);

		/* Setting an overflow slot count to 0 or 1 in newSpace creates a slimbridge.
		   So we must also change the normal slot count so that the object no
		   longer has the overflow header word, which has become the slimbridge. */
		if (numSlots <= 1) {
			if (oopisLessThan(objOop, GIV(oldSpaceStart))) {
				/* rawNumSlotsOf:put: */
				byteAtput((void *)(objOop + (numSlotsFieldByteOffset())),numSlots);
				hackSlimBridgeToat(objOop, objOop - 8 /* allocationUnit */);
			}
		}
	}
	else {
		assert(numSlots < (numSlotsMask()));

		/* rawNumSlotsOf:put: */
		byteAtput((void *)(objOop + (numSlotsFieldByteOffset())),numSlots);
	}

	/* {self addressAfter: objOop. delta. (self addressAfter: objOop) + delta. followingAddress} collect: #hex */
	delta = followingAddress - (addressAfter(objOop));
	assert((delta >= (allocationUnit()))
	 && ((delta % (allocationUnit())) == 0));
	if (delta == 8 /* allocationUnit */) {
		assert(!((isOldObject(objOop))));
		delta = 0;
		if (followingAddress == GIV(freeStart)) {
			GIV(freeStart) = addressAfter(objOop);
		}
		else {
			hackSlimBridgeToat(followingAddress, addressAfter(objOop));
		}
	}
	else {
		freeChunk = initFreeChunkWithBytesat(delta, addressAfter(objOop));
		assert((objectAfter(objOop)) == freeChunk);
		assert((addressAfter(freeChunk)) == followingAddress);
		if (oopisGreaterThanOrEqualToandLessThan(objOop, GIV(oldSpaceStart), GIV(endOfMemory))) {
			GIV(totalFreeOldSpace) += delta;
			addToFreeListbytes(freeChunk, delta);
		}
		else {
			delta = 0;

			/* begin setClassIndexOf:to: */
			assert((((sixtyFourBitLongsClassIndexPun()) >= 0) && ((sixtyFourBitLongsClassIndexPun()) <= (classIndexMask()))));
			longAtput((void *)(freeChunk),((longAt((void *)(freeChunk))) & (~(usqIntptr_t)(classIndexMask()))) + (sixtyFourBitLongsClassIndexPun()));

			/* begin setFormatOf:to: */
			assert((((firstLongFormat()) >= 0) && ((firstLongFormat()) <= (formatMask()))));
			byteAtput((void *)(freeChunk + (formatFieldByteOffset())),((byteAt((void *)(freeChunk + (formatFieldByteOffset())))) & (0xFF - (formatMask()))) + (firstLongFormat()));
		}
	}

	/* begin normalisedFormatFor:indexableSize: */
	instFormat = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
	if (instFormat >= (firstByteFormat())) {
		formatSqInt = instFormat & -8;
		goto l3;
	}

	/* this is likely the common case */
	if (instFormat <= (sixtyFourBitIndexableFormat())) {
		formatSqInt = instFormat;
		goto l3;
	}
	if (instFormat < (firstShortFormat())) {
		formatSqInt = instFormat & -2;
		goto l3;
	}
	formatSqInt = instFormat & -4;
	/* end classFormatFromInstFormat: */
l3:
	formatSqInt = ((formatSqInt == (firstLongFormat()))
		 && (((indexableSize & 1) != 0))
				? formatSqInt + 1
				: formatSqInt);

	/* begin setFormatOf:to: */
	assert(((formatSqInt >= 0) && (formatSqInt <= (formatMask()))));
	byteAtput((void *)(objOop + (formatFieldByteOffset())),((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (0xFF - (formatMask()))) + formatSqInt);
	return delta;
}