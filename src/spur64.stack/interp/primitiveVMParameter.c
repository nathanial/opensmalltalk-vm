/* Extracted from interp.c:70865 (function primitiveVMParameter). */

static void
primitiveVMParameter(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    double aProportion;
    sqIntptr_t arg;
    sqInt argOop;
    sqLong delta;
    sqInt index;
    usqInt limit;
    int mustFlush;
    sqInt oldPrimitiveDoMixedArithmetic;
    sqInt oop;
    sqInt paramsArraySize;
    sqInt result;
    sqInt resultSqInt;
    char *sp;
    sqInt tagBits;
    sqInt value;
    sqInt valuePointer;

	paramsArraySize = 76;
	if (!GIV(argumentCount)) {
		/* begin primitiveAllVMParameters: */
		resultSqInt = instantiateClassindexableSize(longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassArray) << (shiftForWord())))))), paramsArraySize);
		valuePointer = positive64BitIntegerFor(
				(/* begin oldSpaceSize *//* begin totalOldSpaceSize */
					assert((totalBytesInSegments()) == GIV(totalHeapSizeIncludingBridges)),
				GIV(totalHeapSizeIncludingBridges)));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(0, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (0U << (shiftForWord()))),valuePointer);
		valuePointer = (((usqInt)((GIV(freeStart) - (((GIV(eden)).start))) + (GIV(pastSpaceStart) - (((GIV(pastSpace)).start)))) << 3) | 1);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(1, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (1U << (shiftForWord()))),valuePointer);
		valuePointer = positive64BitIntegerFor((newSpaceCapacity()) + ((/* begin totalOldSpaceSize */
	assert((totalBytesInSegments()) == GIV(totalHeapSizeIncludingBridges)),
GIV(totalHeapSizeIncludingBridges))));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(2, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (2U << (shiftForWord()))),valuePointer);

		/* objectMemory storePointerUnchecked: 3	ofObject: result withValue: objectMemory nilObject was allocationCount
		   objectMemory storePointerUnchecked: 4	ofObject: result withValue: objectMemory nilObject allocationsBetweenGCs */
		valuePointer = (((usqInt)(((sqInt)(((scavengerTenuringThreshold()) * (((GIV(pastSpace).limit)) - ((GIV(pastSpace).start)))) / (8 * BytesPerOop)))) << 3) | 1);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(5, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (5U << (shiftForWord()))),valuePointer);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(6, resultSqInt, (((usqInt)GIV(statFullGCs) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (6U << (shiftForWord()))),(((usqInt)GIV(statFullGCs) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(7, resultSqInt, ((((GIV(statFullGCUsecs) + 500) / 1000) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (7U << (shiftForWord()))),((((GIV(statFullGCUsecs) + 500) / 1000) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(8, resultSqInt, (((usqInt)GIV(statScavenges) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (8U << (shiftForWord()))),(((usqInt)GIV(statScavenges) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(9, resultSqInt, ((((GIV(statScavengeGCUsecs) + 500) / 1000) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (9U << (shiftForWord()))),((((GIV(statScavengeGCUsecs) + 500) / 1000) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(10, resultSqInt, (((usqInt)GIV(statTenures) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (10U << (shiftForWord()))),(((usqInt)GIV(statTenures) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(11, resultSqInt, (((usqInt)eventTraceMask << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (11U << (shiftForWord()))),(((usqInt)eventTraceMask << 3) | 1));
		valuePointer = 
#      if VM_TICKER
				positive64BitIntegerFor(ioVMTickerStartUSecs())
#      else
				ConstZero
#      endif
				;

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(12, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (12U << (shiftForWord()))),valuePointer);
		valuePointer = 
#      if VM_TICKER
				positive64BitIntegerFor(ioVMTickerCount())
#      else
				ConstZero
#      endif
				;

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(13, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (13U << (shiftForWord()))),valuePointer);
		valuePointer = 
#      if VM_TICKER
				positive64BitIntegerFor(ioVMTickeeCallCount())
#      else
				ConstZero
#      endif
				;

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(14, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (14U << (shiftForWord()))),valuePointer);
		valuePointer = positive64BitIntegerFor(GIV(statIdleUsecs));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(15, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (15U << (shiftForWord()))),valuePointer);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(16, resultSqInt, ConstZero));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (16U << (shiftForWord()))),ConstZero);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(17, resultSqInt, ((((GIV(statCompactionUsecs) + 500) / 1000) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (17U << (shiftForWord()))),((((GIV(statCompactionUsecs) + 500) / 1000) << 3) | 1));
		valuePointer = (((usqInt)(GIV(scavengeThreshold) - (((GIV(eden)).start))) << 3) | 1);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(18, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (18U << (shiftForWord()))),valuePointer);
		valuePointer = positive64BitIntegerFor(ioUTCStartMicroseconds());

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(19, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (19U << (shiftForWord()))),valuePointer);
		valuePointer = (((usqInt)(rootTableCount()) << 3) | 1);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(20, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (20U << (shiftForWord()))),valuePointer);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(21, resultSqInt, (((usqInt)GIV(statRootTableOverflows) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (21U << (shiftForWord()))),(((usqInt)GIV(statRootTableOverflows) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(22, resultSqInt, (((usqInt)extraVMMemory << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (22U << (shiftForWord()))),(((usqInt)extraVMMemory << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(23, resultSqInt, (((usqInt)GIV(shrinkThreshold) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (23U << (shiftForWord()))),(((usqInt)GIV(shrinkThreshold) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(24, resultSqInt, (((usqInt)GIV(growHeadroom) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (24U << (shiftForWord()))),(((usqInt)GIV(growHeadroom) << 3) | 1));
		valuePointer = (((usqInt)(ioHeartbeatMilliseconds()) << 3) | 1);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(25, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (25U << (shiftForWord()))),valuePointer);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(26, resultSqInt, (((usqInt)GIV(statMarkCount) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (26U << (shiftForWord()))),(((usqInt)GIV(statMarkCount) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(27, resultSqInt, (((usqInt)0 /* statSweepCount */ << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (27U << (shiftForWord()))),(((usqInt)0 /* statSweepCount */ << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(28, resultSqInt, (((usqInt)0 /* statMkFwdCount */ << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (28U << (shiftForWord()))),(((usqInt)0 /* statMkFwdCount */ << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(29, resultSqInt, (((usqInt)GIV(statCompactPassCount) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (29U << (shiftForWord()))),(((usqInt)GIV(statCompactPassCount) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(30, resultSqInt, (((usqInt)GIV(statGrowMemory) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (30U << (shiftForWord()))),(((usqInt)GIV(statGrowMemory) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(0x1F, resultSqInt, (((usqInt)GIV(statShrinkMemory) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (0x1FU << (shiftForWord()))),(((usqInt)GIV(statShrinkMemory) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(32, resultSqInt, (((usqInt)GIV(statRootTableCount) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (32U << (shiftForWord()))),(((usqInt)GIV(statRootTableCount) << 3) | 1));
		valuePointer = positive64BitIntegerFor(currentAllocatedBytes());

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(33, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (33U << (shiftForWord()))),valuePointer);

		/* was statAllocationCount */

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(34, resultSqInt, (((usqInt)GIV(statSurvivorCount) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (34U << (shiftForWord()))),(((usqInt)GIV(statSurvivorCount) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(35, resultSqInt, ((((GIV(statGCEndUsecs) / 1000) & MillisecondClockMask) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (35U << (shiftForWord()))),((((GIV(statGCEndUsecs) / 1000) & MillisecondClockMask) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(36, resultSqInt, (((usqInt)0 /* statSpecialMarkCount */ << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (36U << (shiftForWord()))),(((usqInt)0 /* statSpecialMarkCount */ << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(37, resultSqInt, ((((GIV(statIGCDeltaUsecs) + 500) / 1000) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (37U << (shiftForWord()))),((((GIV(statIGCDeltaUsecs) + 500) / 1000) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(38, resultSqInt, (((usqInt)GIV(statPendingFinalizationSignals) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (38U << (shiftForWord()))),(((usqInt)GIV(statPendingFinalizationSignals) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(39, resultSqInt, (((usqInt)BytesPerWord << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (39U << (shiftForWord()))),(((usqInt)BytesPerWord << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(40, resultSqInt, (((usqInt)68021 /* imageFormatVersion */ << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (40U << (shiftForWord()))),(((usqInt)68021 /* imageFormatVersion */ << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(41, resultSqInt, (((usqInt)GIV(numStackPages) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (41U << (shiftForWord()))),(((usqInt)GIV(numStackPages) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(42, resultSqInt, (((usqInt)desiredNumStackPages << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (42U << (shiftForWord()))),(((usqInt)desiredNumStackPages << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(43, resultSqInt, (((usqInt)(((GIV(eden).limit)) - ((GIV(eden).start))) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (43U << (shiftForWord()))),(((usqInt)(((GIV(eden).limit)) - ((GIV(eden).start))) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(44, resultSqInt, (((usqInt)desiredEdenBytes << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (44U << (shiftForWord()))),(((usqInt)desiredEdenBytes << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(45, resultSqInt, getCogCodeSize()));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (45U << (shiftForWord()))),GIV(nilObj));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(46, resultSqInt, getDesiredCogCodeSize()));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (46U << (shiftForWord()))),GIV(nilObj));
		valuePointer = (((usqInt)(((usqInt)((getImageHeaderFlags()))) >> 2) << 3) | 1);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(47, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (47U << (shiftForWord()))),valuePointer);
		valuePointer = (((usqInt)(ioGetMaxExtSemTableSize()) << 3) | 1);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(48, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (48U << (shiftForWord()))),valuePointer);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(49, resultSqInt, getMaxLiteralCountForCompile()));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (49U << (shiftForWord()))),GIV(nilObj));
		valuePointer = (((usqInt)(((GIV(oldImageBaseAddress) != GIV(oldSpaceStart)
		? 1
		: 0)) + ((sqImageFileIsEmbedded()
		? 2
		: 0))) << 3) | 1);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(50, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (50U << (shiftForWord()))),valuePointer);
		valuePointer = (((usqInt)(rootTableCapacity()) << 3) | 1);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(51, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (51U << (shiftForWord()))),valuePointer);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(52, resultSqInt, (((usqInt)GIV(numSegments) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (52U << (shiftForWord()))),(((usqInt)GIV(numSegments) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(53, resultSqInt, ((GIV(totalFreeOldSpace) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (53U << (shiftForWord()))),((GIV(totalFreeOldSpace) << 3) | 1));
		valuePointer = floatObjectOf(getHeapGrowthToSizeGCRatio());

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(54, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (54U << (shiftForWord()))),valuePointer);
		valuePointer = positive64BitIntegerFor(GIV(statProcessSwitch));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(55, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (55U << (shiftForWord()))),valuePointer);
		valuePointer = positive64BitIntegerFor(GIV(statIOProcessEvents));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(56, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (56U << (shiftForWord()))),valuePointer);
		valuePointer = positive64BitIntegerFor(GIV(statForceInterruptCheck));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(57, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (57U << (shiftForWord()))),valuePointer);
		valuePointer = positive64BitIntegerFor(GIV(statCheckForEvents));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(58, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (58U << (shiftForWord()))),valuePointer);
		valuePointer = positive64BitIntegerFor(GIV(statStackOverflow));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(59, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (59U << (shiftForWord()))),valuePointer);
		valuePointer = positive64BitIntegerFor(GIV(statStackPageDivorce));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(60, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (60U << (shiftForWord()))),valuePointer);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(61, resultSqInt, getCodeCompactionCount()));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (61U << (shiftForWord()))),GIV(nilObj));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(0x3E, resultSqInt, getCodeCompactionMSecs()));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (0x3EU << (shiftForWord()))),GIV(nilObj));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(0x3F, resultSqInt, ConstZero));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (0x3FU << (shiftForWord()))),ConstZero);
		valuePointer = getCogVMFeatureFlags();

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(64, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (64U << (shiftForWord()))),valuePointer);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(65, resultSqInt, (((usqInt)GIV(bytesPerPage) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (65U << (shiftForWord()))),(((usqInt)GIV(bytesPerPage) << 3) | 1));
		valuePointer = positive64BitIntegerFor(maxOldSpaceSize);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(66, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (66U << (shiftForWord()))),valuePointer);
		valuePointer = floatObjectOf(statAverageLivePagesWhenMapping());

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(67, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (67U << (shiftForWord()))),valuePointer);
		valuePointer = (((usqInt)(GIV(statMaxPageCountWhenMapping)) << 3) | 1);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(68, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (68U << (shiftForWord()))),valuePointer);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(69, resultSqInt, (((usqInt)VM_PROXY_MAJOR /* vmProxyMajorVersion */ << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (69U << (shiftForWord()))),(((usqInt)VM_PROXY_MAJOR /* vmProxyMajorVersion */ << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(70, resultSqInt, (((usqInt)VM_PROXY_MINOR /* vmProxyMinorVersion */ << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (70U << (shiftForWord()))),(((usqInt)VM_PROXY_MINOR /* vmProxyMinorVersion */ << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(71, resultSqInt, ((((GIV(statMarkUsecs) + 500) / 1000) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (71U << (shiftForWord()))),((((GIV(statMarkUsecs) + 500) / 1000) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(72, resultSqInt, ((((GIV(statSweepUsecs) + 500) / 1000) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (72U << (shiftForWord()))),((((GIV(statSweepUsecs) + 500) / 1000) << 3) | 1));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(73, resultSqInt, (((usqInt)((GIV(statMaxAllocSegmentTime) + 500) / 1000) << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (73U << (shiftForWord()))),(((usqInt)((GIV(statMaxAllocSegmentTime) + 500) / 1000) << 3) | 1));
		valuePointer = /* booleanObjectOf: */
				(primitiveDoMixedArithmetic
					? GIV(trueObj)
					: GIV(falseObj));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(74, resultSqInt, valuePointer));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (74U << (shiftForWord()))),valuePointer);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(resultSqInt))
		 && (!(isForwarded(resultSqInt))));
		assert(validStorePointerUncheckedArgs(75, resultSqInt, (((usqInt)-1 /* minimumUnusedHeadroom */ << 3) | 1)));
		longAtput((void *)((resultSqInt + BaseHeaderSize) + (75U << (shiftForWord()))),(((usqInt)-1 /* minimumUnusedHeadroom */ << 3) | 1));
		beRootIfOld(resultSqInt);

		/* begin methodReturnValue: */
		assert(!((failed())));
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),resultSqInt);
		GIV(stackPointer) = sp;
		return;
	}
	if (GIV(argumentCount) > 2) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadNumArgs;
		return;
	}

	/* index read & checks */
	index = longAt(GIV(stackPointer) + (((GIV(argumentCount) == 1
		? 0
		: 1)) * BytesPerWord));
	if (!((((index) & 7) == 1))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	index = (index >> 3);
	if ((index < 1)
	 || (index > paramsArraySize)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		return;
	}
	if (GIV(argumentCount) == 1) {
		/* begin primitiveGetVMParameter: */
		switch (index) {
		case 1:
			result = positive64BitIntegerFor(
					(/* begin oldSpaceSize *//* begin totalOldSpaceSize */
						assert((totalBytesInSegments()) == GIV(totalHeapSizeIncludingBridges)),
					GIV(totalHeapSizeIncludingBridges)));
			goto l1;
			break;
		case 2:
			result = (((usqInt)((GIV(freeStart) - (((GIV(eden)).start))) + (GIV(pastSpaceStart) - (((GIV(pastSpace)).start)))) << 3) | 1);
			goto l1;
			break;
		case 3:
			result = positive64BitIntegerFor((newSpaceCapacity()) + ((/* begin totalOldSpaceSize */
	assert((totalBytesInSegments()) == GIV(totalHeapSizeIncludingBridges)),
GIV(totalHeapSizeIncludingBridges))));
			goto l1;
			break;
		case 6:
			result = (((usqInt)(((sqInt)(((scavengerTenuringThreshold()) * (((GIV(pastSpace).limit)) - ((GIV(pastSpace).start)))) / (8 * BytesPerOop)))) << 3) | 1);
			goto l1;
			break;
		case 7:
			result = (((usqInt)GIV(statFullGCs) << 3) | 1);
			goto l1;
			break;
		case 8:
			result = ((((GIV(statFullGCUsecs) + 500) / 1000) << 3) | 1);
			goto l1;
			break;
		case 9:
			result = (((usqInt)GIV(statScavenges) << 3) | 1);
			goto l1;
			break;
		case 10:
			result = ((((GIV(statScavengeGCUsecs) + 500) / 1000) << 3) | 1);
			goto l1;
			break;
		case 11:
			result = (((usqInt)GIV(statTenures) << 3) | 1);
			goto l1;
			break;
		case 12:
			result = (((usqInt)eventTraceMask << 3) | 1);
			goto l1;
			break;
		case 13:
			result = 
#        if VM_TICKER
					positive64BitIntegerFor(ioVMTickerStartUSecs())
#        else
					ConstZero
#        endif
					;
			goto l1;
			break;
		case 14:
			result = 
#        if VM_TICKER
					positive64BitIntegerFor(ioVMTickerCount())
#        else
					ConstZero
#        endif
					;
			goto l1;
			break;
		case 15:
			result = 
#        if VM_TICKER
					positive64BitIntegerFor(ioVMTickeeCallCount())
#        else
					ConstZero
#        endif
					;
			goto l1;
			break;
		case 16:
			result = positive64BitIntegerFor(GIV(statIdleUsecs));
			goto l1;
			break;
		case 17:
			result = ConstZero;
			goto l1;
			break;
		case 18:
			result = ((((GIV(statCompactionUsecs) + 500) / 1000) << 3) | 1);
			goto l1;
			break;
		case 19:
			result = (((usqInt)(GIV(scavengeThreshold) - (((GIV(eden)).start))) << 3) | 1);
			goto l1;
			break;
		case 20:
			result = positive64BitIntegerFor(ioUTCStartMicroseconds());
			goto l1;
			break;
		case 21:
			result = (((usqInt)(rootTableCount()) << 3) | 1);
			goto l1;
			break;
		case 22:
			result = (((usqInt)GIV(statRootTableOverflows) << 3) | 1);
			goto l1;
			break;
		case 23:
			result = (((usqInt)extraVMMemory << 3) | 1);
			goto l1;
			break;
		case 24:
			result = (((usqInt)GIV(shrinkThreshold) << 3) | 1);
			goto l1;
			break;
		case 25:
			result = (((usqInt)GIV(growHeadroom) << 3) | 1);
			goto l1;
			break;
		case 26:
			result = (((usqInt)(ioHeartbeatMilliseconds()) << 3) | 1);
			goto l1;
			break;
		case 27:
			result = (((usqInt)GIV(statMarkCount) << 3) | 1);
			goto l1;
			break;
		case 28:
			result = (((usqInt)0 /* statSweepCount */ << 3) | 1);
			goto l1;
			break;
		case 29:
			result = (((usqInt)0 /* statMkFwdCount */ << 3) | 1);
			goto l1;
			break;
		case 30:
			result = (((usqInt)GIV(statCompactPassCount) << 3) | 1);
			goto l1;
			break;
		case 0x1F:
			result = (((usqInt)GIV(statGrowMemory) << 3) | 1);
			goto l1;
			break;
		case 32:
			result = (((usqInt)GIV(statShrinkMemory) << 3) | 1);
			goto l1;
			break;
		case 33:
			result = (((usqInt)GIV(statRootTableCount) << 3) | 1);
			goto l1;
			break;
		case 34:
			result = positive64BitIntegerFor(currentAllocatedBytes());
			goto l1;
			break;
		case 35:
			result = (((usqInt)GIV(statSurvivorCount) << 3) | 1);
			goto l1;
			break;
		case 36:
			result = ((((GIV(statGCEndUsecs) / 1000) & MillisecondClockMask) << 3) | 1);
			goto l1;
			break;
		case 37:
			result = (((usqInt)0 /* statSpecialMarkCount */ << 3) | 1);
			goto l1;
			break;
		case 38:
			result = ((((GIV(statIGCDeltaUsecs) + 500) / 1000) << 3) | 1);
			goto l1;
			break;
		case 39:
			result = (((usqInt)GIV(statPendingFinalizationSignals) << 3) | 1);
			goto l1;
			break;
		case 40:
			result = (((usqInt)BytesPerWord << 3) | 1);
			goto l1;
			break;
		case 41:
			result = (((usqInt)68021 /* imageFormatVersion */ << 3) | 1);
			goto l1;
			break;
		case 42:
			result = (((usqInt)GIV(numStackPages) << 3) | 1);
			goto l1;
			break;
		case 43:
			result = (((usqInt)desiredNumStackPages << 3) | 1);
			goto l1;
			break;
		case 44:
			result = (((usqInt)(((GIV(eden).limit)) - ((GIV(eden).start))) << 3) | 1);
			goto l1;
			break;
		case 45:
			result = (((usqInt)desiredEdenBytes << 3) | 1);
			goto l1;
			break;
		case 46:
			result = GIV(nilObj);
			goto l1;
			break;
		case 47:
			result = GIV(nilObj);
			goto l1;
			break;
		case 48:
			result = (((usqInt)(((usqInt)((getImageHeaderFlags()))) >> 2) << 3) | 1);
			goto l1;
			break;
		case 49:
			result = (((usqInt)(ioGetMaxExtSemTableSize()) << 3) | 1);
			goto l1;
			break;
		case 50:
			result = GIV(nilObj);
			goto l1;
			break;
		case 51:
			result = (((usqInt)(((GIV(oldImageBaseAddress) != GIV(oldSpaceStart)
		? 1
		: 0)) + ((sqImageFileIsEmbedded()
		? 2
		: 0))) << 3) | 1);
			goto l1;
			break;
		case 52:
			result = (((usqInt)(rootTableCapacity()) << 3) | 1);
			goto l1;
			break;
		case 53:
			result = (((usqInt)GIV(numSegments) << 3) | 1);
			goto l1;
			break;
		case 54:
			result = ((GIV(totalFreeOldSpace) << 3) | 1);
			goto l1;
			break;
		case 55:
			result = floatObjectOf(getHeapGrowthToSizeGCRatio());
			goto l1;
			break;
		case 56:
			result = positive64BitIntegerFor(GIV(statProcessSwitch));
			goto l1;
			break;
		case 57:
			result = positive64BitIntegerFor(GIV(statIOProcessEvents));
			goto l1;
			break;
		case 58:
			result = positive64BitIntegerFor(GIV(statForceInterruptCheck));
			goto l1;
			break;
		case 59:
			result = positive64BitIntegerFor(GIV(statCheckForEvents));
			goto l1;
			break;
		case 60:
			result = positive64BitIntegerFor(GIV(statStackOverflow));
			goto l1;
			break;
		case 61:
			result = positive64BitIntegerFor(GIV(statStackPageDivorce));
			goto l1;
			break;
		case 0x3E:
			result = GIV(nilObj);
			goto l1;
			break;
		case 0x3F:
			result = GIV(nilObj);
			goto l1;
			break;
		case 64:
			result = ConstZero;
			goto l1;
			break;
		case 65:
			result = getCogVMFeatureFlags();
			goto l1;
			break;
		case 66:
			result = (((usqInt)GIV(bytesPerPage) << 3) | 1);
			goto l1;
			break;
		case 67:
			result = positive64BitIntegerFor(maxOldSpaceSize);
			goto l1;
			break;
		case 68:
			result = floatObjectOf(statAverageLivePagesWhenMapping());
			goto l1;
			break;
		case 69:
			result = (((usqInt)(GIV(statMaxPageCountWhenMapping)) << 3) | 1);
			goto l1;
			break;
		case 70:
			result = (((usqInt)VM_PROXY_MAJOR /* vmProxyMajorVersion */ << 3) | 1);
			goto l1;
			break;
		case 71:
			result = (((usqInt)VM_PROXY_MINOR /* vmProxyMinorVersion */ << 3) | 1);
			goto l1;
			break;
		case 72:
			result = ((((GIV(statMarkUsecs) + 500) / 1000) << 3) | 1);
			goto l1;
			break;
		case 73:
			result = ((((GIV(statSweepUsecs) + 500) / 1000) << 3) | 1);
			goto l1;
			break;
		case 74:
			result = (((usqInt)((GIV(statMaxAllocSegmentTime) + 500) / 1000) << 3) | 1);
			goto l1;
			break;
		case 75:
			result = /* booleanObjectOf: */
					(primitiveDoMixedArithmetic
						? GIV(trueObj)
						: GIV(falseObj));
			goto l1;
			break;
		case 76:
			result = (((usqInt)-1 /* minimumUnusedHeadroom */ << 3) | 1);
			goto l1;
			break;
		default:
			result = null;
			goto l1;
		}
		/* end primitiveGetVMParameter: */
l1:
		oop = (!(result)
				? GIV(nilObj)
				: result);

		/* begin methodReturnValue: */
		assert(!((failed())));
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),oop);
		GIV(stackPointer) = sp;
		return;
	}

	/* read VM parameter; written this way to avoid branch limits in V3 bytecode set
	   written thus to enable Slang inlining of primitiveGetVMParameter:
	   write a VM parameter */
	argOop = longAt(GIV(stackPointer));

	/* begin primitiveSetVMParameter:arg: */
	switch (index) {
	case 17:
	case 55:
	case 68:
		if (!((/* isFloatInstance: */
				((tagBits = argOop & (tagMask()))
					? tagBits == (smallFloatTag())
					: ((longAt((void *)(argOop))) & (classIndexMask())) == ClassFloatCompactIndex))
			 || ((((argOop) & 7) == 1)))) {
			GIV(primFailCode) = PrimErrBadArgument;
		}
		break;
	case 67:
		/* begin positiveMachineIntegerValueOf: */
		if ((((argOop) & 7) == 1)) {
			value = (argOop >> 3);
			if (value < 0) {
				/* begin primitiveFail */
				if (!GIV(primFailCode)) {
					GIV(primFailCode) = 1;
				}
				arg = ((usqIntptr_t) null);
				goto l2;
			}
			arg = ((usqIntptr_t) value);
			goto l2;
		}

		/* don't inline the rare case */
		arg = positiveMachineIntegerValueOfObj(argOop);
		/* end positiveMachineIntegerValueOf: */
l2:;
		break;
	case 75:
		/* begin booleanValueOf: */
		if (argOop == GIV(trueObj)) {
			arg = 1;
			goto l3;
		}
		if (argOop == GIV(falseObj)) {
			arg = 0;
			goto l3;
		}

		/* begin success: */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		arg = null;
		/* end booleanValueOf: */
l3:;
		break;
	default:
		arg = (argOop >> 3);
	}
	if (GIV(primFailCode)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		goto l6;
	}

	/* assume failure, then set success for handled indices */

	/* primitiveFailFor: */
	GIV(primFailCode) = PrimErrBadArgument;
	switch (index) {
	case 5:
		break;
	case 6:
		resultSqInt = (((usqInt)(((sqInt)(((scavengerTenuringThreshold()) * (((GIV(pastSpace).limit)) - ((GIV(pastSpace).start)))) / (8 * BytesPerOop)))) << 3) | 1);

		/* begin tenuringThreshold: */
		if (arg < 0) {
			GIV(primFailCode) = PrimErrBadArgument;
			goto l4;
		}
		aProportion = (((double) (arg * (8 * BytesPerOop)) )) / (((double) (((GIV(pastSpace).limit)) - ((GIV(pastSpace).start))) ));

		/* begin scavengerTenuringThreshold: */
		GIV(tenuringProportion) = aProportion;
		GIV(tenureThreshold) = (aProportion == 0.0
					? 0
					: (round(((((GIV(pastSpace).limit)) - ((GIV(pastSpace).start))) * (1.0 - aProportion)))) + ((GIV(pastSpace).start)));
		GIV(primFailCode) = 0;
		/* end tenuringThreshold: */
l4:;
		break;
	case 12:
		resultSqInt = (((usqInt)eventTraceMask << 3) | 1);
		eventTraceMask = arg;

		/* begin initPrimCall */
		GIV(primFailCode) = 0;
		break;
	case 11:
		if (arg >= 0) {
			resultSqInt = (((usqInt)GIV(statTenures) << 3) | 1);

			/* begin statTenures: */
			GIV(statTenures) = arg;
			GIV(primFailCode) = 0;
		}
		break;
	case 17:
		break;
	case 23:
		resultSqInt = (((usqInt)extraVMMemory << 3) | 1);
		extraVMMemory = arg;

		/* begin initPrimCall */
		GIV(primFailCode) = 0;
		break;
	case 24:
		if (arg > 0) {
			resultSqInt = (((usqInt)GIV(shrinkThreshold) << 3) | 1);

			/* begin shrinkThreshold: */
			GIV(shrinkThreshold) = arg;
			GIV(primFailCode) = 0;
		}
		break;
	case 25:
		if (arg > 0) {
			resultSqInt = (((usqInt)GIV(growHeadroom) << 3) | 1);

			/* growHeadroom: */
			GIV(growHeadroom) = arg;

			/* begin initPrimCall */
			GIV(primFailCode) = 0;
		}
		break;
	case 26:
		if (arg >= 0) {
			resultSqInt = (((usqInt)(ioHeartbeatMilliseconds()) << 3) | 1);
			ioSetHeartbeatMilliseconds(arg);

			/* begin initPrimCall */
			GIV(primFailCode) = 0;
		}
		break;
	case 34:
		if (arg >= 0) {
			resultSqInt = positive64BitIntegerFor(currentAllocatedBytes());

			/* begin setCurrentAllocatedBytesTo: */
			delta = (currentAllocatedBytes()) - GIV(statAllocatedBytes);
			GIV(statAllocatedBytes) = arg;
			GIV(oldSpaceUsePriorToScavenge) += delta;
			assert((currentAllocatedBytes()) == arg);

			/* begin initPrimCall */
			GIV(primFailCode) = 0;
		}
		break;
	case 43:
		if (((arg >= 0) && (arg <= 0xFFFF))) {
			resultSqInt = (((usqInt)desiredNumStackPages << 3) | 1);
			desiredNumStackPages = arg;

			/* begin initPrimCall */
			GIV(primFailCode) = 0;
		}
		break;
	case 45:
		if (arg >= 0) {
			resultSqInt = (((usqInt)desiredEdenBytes << 3) | 1);
			desiredEdenBytes = arg;

			/* begin initPrimCall */
			GIV(primFailCode) = 0;
		}
		break;
	case 47:
		break;
	case 48:
		if (arg >= 0) {
			oldPrimitiveDoMixedArithmetic = primitiveDoMixedArithmetic;
			resultSqInt = (((usqInt)(((usqInt)((getImageHeaderFlags()))) >> 2) << 3) | 1);

			/* begin initPrimCall */
			GIV(primFailCode) = 0;
			if ((((usqInt)arg)) > 0x1FF) {
				/* primitiveFailFor: */
				GIV(primFailCode) = PrimErrUnsupported;
				goto l5;
			}

			/* threadAffinity := flags anyMask: 1. specific to CoInterpreterMT */
			GIV(imageHeaderFlags) = (((arg & 1) != 0)
						? GIV(imageHeaderFlags) | 4
						: ((GIV(imageHeaderFlags) | 4) - 4));

			/* flagInterpretedMethods := flags anyMask: 2. specific to CoInterpreter */
			GIV(imageHeaderFlags) = (((arg & 2) != 0)
						? GIV(imageHeaderFlags) | 8
						: ((GIV(imageHeaderFlags) | 8) - 8));
			GIV(preemptionYields) = (!(arg & 4));

			/* noThreadingOfGUIThread := flags anyMask: 8.. specific to CoInterpreterMT */
			GIV(imageHeaderFlags) = (((arg & 8) != 0)
						? GIV(imageHeaderFlags) | 32
						: ((GIV(imageHeaderFlags) | 32) - 32));
			GIV(newFinalization) = ((arg & 16) != 0);
			sendWheelEvents = ((arg & 32) != 0);
			primitiveDoMixedArithmetic = (!(arg & 64));
			GIV(imageHeaderFlags) = (((arg & 128) != 0)
						? GIV(imageHeaderFlags) | 0x200
						: ((GIV(imageHeaderFlags) | 0x200) - 0x200));
			upscaleDisplayIfHighDPI = (!(arg & 0x100));
			/* end setImageHeaderFlags: */
l5:
			if ((GIV(primFailCode) == 0)
			 && (oldPrimitiveDoMixedArithmetic != primitiveDoMixedArithmetic)) {
				/* begin flushMethodCache */
				memset(GIV(methodCache), 0, MethodCacheSize * (sizeof(GIV(methodCache)[0])));

				/* this for primitiveExternalMethod */
				GIV(lastMethodCacheProbeWrite) = 0;

				/* begin flushAtCache */
				memset(GIV(atCache), 0, AtCacheTotalSize * (sizeof(GIV(atCache)[0])));
				goto l6;
			}
		}
		break;
	case 49:
		if (((arg >= 0) && (arg <= 0xFFFF))) {
			resultSqInt = (((usqInt)(ioGetMaxExtSemTableSize()) << 3) | 1);

			/* begin initPrimCall */
			GIV(primFailCode) = 0;
			GIV(maxExtSemTabSizeSet) = 1;
			ioSetMaxExtSemTableSize(arg);
		}
		break;
	case 50:
		break;
	case 55:
		resultSqInt = floatObjectOf(getHeapGrowthToSizeGCRatio());

		/* begin setHeapGrowthToSizeGCRatio: */
		GIV(heapGrowthToSizeGCRatio) = loadFloatOrIntFrom(argOop);
		GIV(primFailCode) = 0;
		break;
	case 67:
		if (arg >= 0) {
			resultSqInt = positive64BitIntegerFor(maxOldSpaceSize);
			limit = arg;

			/* begin setMaxOldSpaceSize: */
			maxOldSpaceSize = limit;
			GIV(primFailCode) = 0;
		}
		break;
	case 68:
		resultSqInt = floatObjectOf(statAverageLivePagesWhenMapping());

		/* begin initPrimCall */
		GIV(primFailCode) = 0;
		if ((loadFloatOrIntFrom(argOop)) == 0.0) {
			GIV(statPageCountWhenMappingSum) = (GIV(statNumMaps) = 0);
		}
		else {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadArgument;
		}
		break;
	case 69:
		if (arg >= 0) {
			resultSqInt = (((usqInt)(GIV(statMaxPageCountWhenMapping)) << 3) | 1);

			/* begin statMaxPageCountWhenMapping: */
			GIV(statMaxPageCountWhenMapping) = arg;
			GIV(primFailCode) = 0;
		}
		break;
	case 74:
		if (arg >= 0) {
			resultSqInt = (((usqInt)((GIV(statMaxAllocSegmentTime) + 500) / 1000) << 3) | 1);

			/* begin statMaxAllocSegmentTime: */
			GIV(statMaxAllocSegmentTime) = arg;
			GIV(primFailCode) = 0;
		}
		break;
	case 75:
		resultSqInt = /* booleanObjectOf: */
				(primitiveDoMixedArithmetic
					? GIV(trueObj)
					: GIV(falseObj));

		/* begin initPrimCall */
		GIV(primFailCode) = 0;
		mustFlush = primitiveDoMixedArithmetic != arg;
		primitiveDoMixedArithmetic = arg;
		if (mustFlush) {
			/* begin flushMethodCache */
			memset(GIV(methodCache), 0, MethodCacheSize * (sizeof(GIV(methodCache)[0])));

			/* this for primitiveExternalMethod */
			GIV(lastMethodCacheProbeWrite) = 0;

			/* begin flushAtCache */
			memset(GIV(atCache), 0, AtCacheTotalSize * (sizeof(GIV(atCache)[0])));
			goto l6;
		}
		break;
	default:
;
	}

	/* disable inlining...
	   disable inlining...
	   disable inlining... */
	if (GIV(primFailCode)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrInappropriate;
	}
	else {
		/* begin methodReturnValue: */
		assert(!((failed())));
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),resultSqInt);
		GIV(stackPointer) = sp;
	}
	/* end primitiveSetVMParameter:arg: */
l6:;
}