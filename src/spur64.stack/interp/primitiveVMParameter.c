/* Extracted from interp.c:70865 (function primitiveVMParameter). */

/*	Behaviour depends on argument count:
        0 args:	return an Array of VM parameter values;
        1 arg:	return the indicated VM parameter;
        2 args:	set the VM indicated parameter.
        VM parameters are numbered as follows:
        1	end (v3)/size(Spur) of old-space (0-based, read-only)
        2	end (v3)/size(Spur) of young/new-space (read-only)
        3	end (v3)/size(Spur) of heap (read-only)
        4	nil (was allocationCount (read-only))
        5	nil (was allocations between GCs (read-write)
        6	survivor count tenuring threshold (read-write)
        7	full GCs since startup (read-only)
        8	total milliseconds in full GCs since startup (read-only)
        9	incremental GCs (SqueakV3) or scavenges (Spur) since startup
   (read-only) 10	total milliseconds in incremental GCs (SqueakV3) or
   scavenges (Spur) since startup (read-only) 11	tenures of surving
   objects since startup or reset (read-write) 12-20 were specific to ikp's
   JITTER VM, now 12 16 open for use 13	if started, the start time in utc
   microseconds of the high-priority ticker 14	if started, the number of
   checkHighPriorityTickees calls 15	if started, the number of tickee calls
   from checkHighPriorityTickees 16	total microseconds at idle since
   start-up (if non-zero) 17	fraction of the code zone to use (Sista only;
   used to control code zone use to preserve sendAndBranchData on counter
   tripped callback) 18	total milliseconds in compaction phase of full GC since
   start-up (Spur only) 19	scavenge threshold, the effective size of eden.
   When eden fills to the threshold a scavenge is scheduled. Newer Spur VMs
   only. 20	utc microseconds at VM start-up (actually at time
   initialization, which precedes image load). 21	root/remembered table
   size (occupancy) (read-only) 22	root table overflows since startup
   (read-only) 23	bytes of extra memory to reserve for VM buffers,
   plugins, etc (stored in image file header). 24	memory threshold above
   which shrinking object memory (rw) 25	memory headroom when growing
   object memory (rw) 26	interruptChecksEveryNms - force an
   ioProcessEvents every N milliseconds (rw) 27	number of times mark loop
   iterated for current IGC/FGC (read-only) includes ALL marking 28	number
   of times sweep loop iterated for current IGC/FGC (read-only) 29	number
   of times make forward loop iterated for current IGC/FGC (read-only) 30
   number of times compact move loop iterated for current IGC/FGC (read-only) 31
   number of grow memory requests (read-only) 32	number of shrink memory
   requests (read-only) 33	number of root table entries used for current
   IGC/FGC (read-only) 34	Spur: bytes allocated in total since start-up or
   reset (read-write) (Used to be number of allocations done before current
   IGC/FGC (read-only)) 35	number of survivor objects after current IGC/FGC
   (read-only) 36	millisecond clock when current IGC/FGC completed
   (read-only) 37	number of marked objects for Roots of the world, not
   including Root Table entries for current IGC/FGC (read-only) 38
   milliseconds taken by current IGC (read-only) 39	Number of finalization
   signals for Weak Objects pending when current IGC/FGC completed (read-only)
        40	BytesPerOop for this image
        41	imageFormatVersion for the VM
        42	number of stack pages in use
        43	desired number of stack pages (stored in image file header, max
   65535) 44	size of eden, in bytes 45	desired size of eden, in bytes
   (stored in image file header) 46	machine code zone size, in bytes (Cog
   only; otherwise nil) 47	desired machine code zone size (stored in image
   file header; Cog only; otherwise nil) 48	various header flags. See
   getImageHeaderFlags. 49	max size the image promises to grow the external
   semaphore table to (0 sets to default, which is 256 as of writing)

                50	max literal count for JIT compile (stored in image file
   header; Cog only; otherwise nil) 51 nil; reserved for VM parameters that
   persist in the image (such as eden above) 52	root/remembered table capacity
        53	number of segments (Spur only; otherwise nil)
        54	total size of free old space (Spur only, otherwise nil)
        55	ratio of growth and image size at or above which a GC will be
   performed post scavenge 56	number of process switches since startup
   (read-only) 57	number of ioProcessEvents calls since startup
   (read-only) 58	number of ForceInterruptCheck calls since startup
   (read-only) 59	number of check event calls since startup (read-only) 60
   number of stack page overflows since startup (read-only) 61	number of stack
   page divorces since startup (read-only) 62	compiled code compactions since
   startup (read-only; Cog only; otherwise nil) 63	total milliseconds in
   compiled code compactions since startup (read-only; Cog only; otherwise nil)
        64	the number of methods that currently have jitted machine-code
        65	various VM feature flags; see getCogVMFeatureFlags
        66	the byte size of a stack page
        67	the max allowed size of old space (Spur only; nil otherwise; 0
   implies no limit except that of the underlying platform) 68	the average
   number of live stack pages when scanned by GC (at scavenge/gc/become et al)
   (read-write) 69	the maximum number of live stack pages when scanned by
   GC (at scavenge/gc/become et al) (read-write) 70	the vmProxyMajorVersion
   (the interpreterProxy VM_MAJOR_VERSION) 71	the vmProxyMinorVersion (the
   interpreterProxy VM_MINOR_VERSION) 72 total milliseconds in full GCs Mark
   phase since startup (read-only) 73 total milliseconds in full GCs Sweep phase
   since startup (read-only, can be 0 depending on compactors) 74 maximum pause
   time due to segment allocation 75 whether the arithmetic primitives perform
   conversion in case of mixed SmallInteger/Float (true) or fail (false) 76 the
   minimum unused headroom in all stack pages; Cog VMs only

        Note: Thanks to Ian Piumarta for this primitive. */

/* StackInterpreterPrimitives>>#primitiveVMParameter */

static void primitiveVMParameter(void) {
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
  if (!argumentCount) {
    /* begin primitiveAllVMParameters: */
    resultSqInt = instantiateClassindexableSize(
        longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                        ((((usqInt)(ClassArray) << (shiftForWord())))))),
        paramsArraySize);
    valuePointer = positive64BitIntegerFor(
        (/* begin oldSpaceSize */ /* begin totalOldSpaceSize */
         assert((totalBytesInSegments()) == totalHeapSizeIncludingBridges),
         totalHeapSizeIncludingBridges));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(0, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (0U << (shiftForWord()))),
        valuePointer);
    valuePointer = (((usqInt)((freeStart - (((eden).start))) +
                              (pastSpaceStart - (((pastSpace).start))))
                     << 3) |
                    1);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(1, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (1U << (shiftForWord()))),
        valuePointer);
    valuePointer = positive64BitIntegerFor(
        (newSpaceCapacity()) +
        ((/* begin totalOldSpaceSize */
          assert((totalBytesInSegments()) == totalHeapSizeIncludingBridges),
          totalHeapSizeIncludingBridges)));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(2, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (2U << (shiftForWord()))),
        valuePointer);

    /* objectMemory storePointerUnchecked: 3	ofObject: result withValue:
       objectMemory nilObject was allocationCount objectMemory
       storePointerUnchecked: 4	ofObject: result withValue: objectMemory
       nilObject allocationsBetweenGCs */
    valuePointer =
        (((usqInt)(((sqInt)(((scavengerTenuringThreshold()) *
                             (((pastSpace.limit)) - ((pastSpace.start)))) /
                            (8 * BytesPerOop))))
          << 3) |
         1);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(5, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (5U << (shiftForWord()))),
        valuePointer);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(6, resultSqInt,
                                          (((usqInt)statFullGCs << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (6U << (shiftForWord()))),
        (((usqInt)statFullGCs << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        7, resultSqInt, ((((statFullGCUsecs + 500) / 1000) << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (7U << (shiftForWord()))),
        ((((statFullGCUsecs + 500) / 1000) << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(8, resultSqInt,
                                          (((usqInt)statScavenges << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (8U << (shiftForWord()))),
        (((usqInt)statScavenges << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        9, resultSqInt, ((((statScavengeGCUsecs + 500) / 1000) << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (9U << (shiftForWord()))),
        ((((statScavengeGCUsecs + 500) / 1000) << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(10, resultSqInt,
                                          (((usqInt)statTenures << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (10U << (shiftForWord()))),
        (((usqInt)statTenures << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(11, resultSqInt,
                                          (((usqInt)eventTraceMask << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (11U << (shiftForWord()))),
        (((usqInt)eventTraceMask << 3) | 1));
    valuePointer =
#if VM_TICKER
        positive64BitIntegerFor(ioVMTickerStartUSecs())
#else
        ConstZero
#endif
        ;

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(12, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (12U << (shiftForWord()))),
        valuePointer);
    valuePointer =
#if VM_TICKER
        positive64BitIntegerFor(ioVMTickerCount())
#else
        ConstZero
#endif
        ;

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(13, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (13U << (shiftForWord()))),
        valuePointer);
    valuePointer =
#if VM_TICKER
        positive64BitIntegerFor(ioVMTickeeCallCount())
#else
        ConstZero
#endif
        ;

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(14, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (14U << (shiftForWord()))),
        valuePointer);
    valuePointer = positive64BitIntegerFor(statIdleUsecs);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(15, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (15U << (shiftForWord()))),
        valuePointer);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(16, resultSqInt, ConstZero));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (16U << (shiftForWord()))),
        ConstZero);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        17, resultSqInt, ((((statCompactionUsecs + 500) / 1000) << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (17U << (shiftForWord()))),
        ((((statCompactionUsecs + 500) / 1000) << 3) | 1));
    valuePointer = (((usqInt)(scavengeThreshold - (((eden).start))) << 3) | 1);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(18, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (18U << (shiftForWord()))),
        valuePointer);
    valuePointer = positive64BitIntegerFor(ioUTCStartMicroseconds());

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(19, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (19U << (shiftForWord()))),
        valuePointer);
    valuePointer = (((usqInt)(rootTableCount()) << 3) | 1);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(20, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (20U << (shiftForWord()))),
        valuePointer);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        21, resultSqInt, (((usqInt)statRootTableOverflows << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (21U << (shiftForWord()))),
        (((usqInt)statRootTableOverflows << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(22, resultSqInt,
                                          (((usqInt)extraVMMemory << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (22U << (shiftForWord()))),
        (((usqInt)extraVMMemory << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        23, resultSqInt, (((usqInt)shrinkThreshold << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (23U << (shiftForWord()))),
        (((usqInt)shrinkThreshold << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(24, resultSqInt,
                                          (((usqInt)growHeadroom << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (24U << (shiftForWord()))),
        (((usqInt)growHeadroom << 3) | 1));
    valuePointer = (((usqInt)(ioHeartbeatMilliseconds()) << 3) | 1);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(25, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (25U << (shiftForWord()))),
        valuePointer);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(26, resultSqInt,
                                          (((usqInt)statMarkCount << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (26U << (shiftForWord()))),
        (((usqInt)statMarkCount << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        27, resultSqInt, (((usqInt)0 /* statSweepCount */ << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (27U << (shiftForWord()))),
        (((usqInt)0 /* statSweepCount */ << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        28, resultSqInt, (((usqInt)0 /* statMkFwdCount */ << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (28U << (shiftForWord()))),
        (((usqInt)0 /* statMkFwdCount */ << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        29, resultSqInt, (((usqInt)statCompactPassCount << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (29U << (shiftForWord()))),
        (((usqInt)statCompactPassCount << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(30, resultSqInt,
                                          (((usqInt)statGrowMemory << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (30U << (shiftForWord()))),
        (((usqInt)statGrowMemory << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        0x1F, resultSqInt, (((usqInt)statShrinkMemory << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (0x1FU << (shiftForWord()))),
        (((usqInt)statShrinkMemory << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        32, resultSqInt, (((usqInt)statRootTableCount << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (32U << (shiftForWord()))),
        (((usqInt)statRootTableCount << 3) | 1));
    valuePointer = positive64BitIntegerFor(currentAllocatedBytes());

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(33, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (33U << (shiftForWord()))),
        valuePointer);

    /* was statAllocationCount */

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        34, resultSqInt, (((usqInt)statSurvivorCount << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (34U << (shiftForWord()))),
        (((usqInt)statSurvivorCount << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        35, resultSqInt,
        ((((statGCEndUsecs / 1000) & MillisecondClockMask) << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (35U << (shiftForWord()))),
        ((((statGCEndUsecs / 1000) & MillisecondClockMask) << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        36, resultSqInt, (((usqInt)0 /* statSpecialMarkCount */ << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (36U << (shiftForWord()))),
        (((usqInt)0 /* statSpecialMarkCount */ << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        37, resultSqInt, ((((statIGCDeltaUsecs + 500) / 1000) << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (37U << (shiftForWord()))),
        ((((statIGCDeltaUsecs + 500) / 1000) << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        38, resultSqInt, (((usqInt)statPendingFinalizationSignals << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (38U << (shiftForWord()))),
        (((usqInt)statPendingFinalizationSignals << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(39, resultSqInt,
                                          (((usqInt)BytesPerWord << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (39U << (shiftForWord()))),
        (((usqInt)BytesPerWord << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        40, resultSqInt, (((usqInt)68021 /* imageFormatVersion */ << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (40U << (shiftForWord()))),
        (((usqInt)68021 /* imageFormatVersion */ << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(41, resultSqInt,
                                          (((usqInt)numStackPages << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (41U << (shiftForWord()))),
        (((usqInt)numStackPages << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        42, resultSqInt, (((usqInt)desiredNumStackPages << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (42U << (shiftForWord()))),
        (((usqInt)desiredNumStackPages << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        43, resultSqInt,
        (((usqInt)(((eden.limit)) - ((eden.start))) << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (43U << (shiftForWord()))),
        (((usqInt)(((eden.limit)) - ((eden.start))) << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        44, resultSqInt, (((usqInt)desiredEdenBytes << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (44U << (shiftForWord()))),
        (((usqInt)desiredEdenBytes << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(45, resultSqInt, getCogCodeSize()));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (45U << (shiftForWord()))),
        nilObj);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(46, resultSqInt,
                                          getDesiredCogCodeSize()));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (46U << (shiftForWord()))),
        nilObj);
    valuePointer =
        (((usqInt)(((usqInt)((getImageHeaderFlags()))) >> 2) << 3) | 1);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(47, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (47U << (shiftForWord()))),
        valuePointer);
    valuePointer = (((usqInt)(ioGetMaxExtSemTableSize()) << 3) | 1);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(48, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (48U << (shiftForWord()))),
        valuePointer);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(49, resultSqInt,
                                          getMaxLiteralCountForCompile()));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (49U << (shiftForWord()))),
        nilObj);
    valuePointer = (((usqInt)(((oldImageBaseAddress != oldSpaceStart ? 1 : 0)) +
                              ((sqImageFileIsEmbedded() ? 2 : 0)))
                     << 3) |
                    1);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(50, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (50U << (shiftForWord()))),
        valuePointer);
    valuePointer = (((usqInt)(rootTableCapacity()) << 3) | 1);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(51, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (51U << (shiftForWord()))),
        valuePointer);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(52, resultSqInt,
                                          (((usqInt)numSegments << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (52U << (shiftForWord()))),
        (((usqInt)numSegments << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(53, resultSqInt,
                                          ((totalFreeOldSpace << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (53U << (shiftForWord()))),
        ((totalFreeOldSpace << 3) | 1));
    valuePointer = floatObjectOf(getHeapGrowthToSizeGCRatio());

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(54, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (54U << (shiftForWord()))),
        valuePointer);
    valuePointer = positive64BitIntegerFor(statProcessSwitch);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(55, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (55U << (shiftForWord()))),
        valuePointer);
    valuePointer = positive64BitIntegerFor(statIOProcessEvents);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(56, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (56U << (shiftForWord()))),
        valuePointer);
    valuePointer = positive64BitIntegerFor(statForceInterruptCheck);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(57, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (57U << (shiftForWord()))),
        valuePointer);
    valuePointer = positive64BitIntegerFor(statCheckForEvents);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(58, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (58U << (shiftForWord()))),
        valuePointer);
    valuePointer = positive64BitIntegerFor(statStackOverflow);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(59, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (59U << (shiftForWord()))),
        valuePointer);
    valuePointer = positive64BitIntegerFor(statStackPageDivorce);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(60, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (60U << (shiftForWord()))),
        valuePointer);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(61, resultSqInt,
                                          getCodeCompactionCount()));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (61U << (shiftForWord()))),
        nilObj);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(0x3E, resultSqInt,
                                          getCodeCompactionMSecs()));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (0x3EU << (shiftForWord()))),
        nilObj);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(0x3F, resultSqInt, ConstZero));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (0x3FU << (shiftForWord()))),
        ConstZero);
    valuePointer = getCogVMFeatureFlags();

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(64, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (64U << (shiftForWord()))),
        valuePointer);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(65, resultSqInt,
                                          (((usqInt)bytesPerPage << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (65U << (shiftForWord()))),
        (((usqInt)bytesPerPage << 3) | 1));
    valuePointer = positive64BitIntegerFor(maxOldSpaceSize);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(66, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (66U << (shiftForWord()))),
        valuePointer);
    valuePointer = floatObjectOf(statAverageLivePagesWhenMapping());

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(67, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (67U << (shiftForWord()))),
        valuePointer);
    valuePointer = (((usqInt)(statMaxPageCountWhenMapping) << 3) | 1);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(68, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (68U << (shiftForWord()))),
        valuePointer);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        69, resultSqInt,
        (((usqInt)VM_PROXY_MAJOR /* vmProxyMajorVersion */ << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (69U << (shiftForWord()))),
        (((usqInt)VM_PROXY_MAJOR /* vmProxyMajorVersion */ << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        70, resultSqInt,
        (((usqInt)VM_PROXY_MINOR /* vmProxyMinorVersion */ << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (70U << (shiftForWord()))),
        (((usqInt)VM_PROXY_MINOR /* vmProxyMinorVersion */ << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        71, resultSqInt, ((((statMarkUsecs + 500) / 1000) << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (71U << (shiftForWord()))),
        ((((statMarkUsecs + 500) / 1000) << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        72, resultSqInt, ((((statSweepUsecs + 500) / 1000) << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (72U << (shiftForWord()))),
        ((((statSweepUsecs + 500) / 1000) << 3) | 1));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        73, resultSqInt,
        (((usqInt)((statMaxAllocSegmentTime + 500) / 1000) << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (73U << (shiftForWord()))),
        (((usqInt)((statMaxAllocSegmentTime + 500) / 1000) << 3) | 1));
    valuePointer = /* booleanObjectOf: */
        (primitiveDoMixedArithmetic ? trueObj : falseObj);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(74, resultSqInt, valuePointer));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (74U << (shiftForWord()))),
        valuePointer);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(resultSqInt)) && (!(isForwarded(resultSqInt))));
    assert(validStorePointerUncheckedArgs(
        75, resultSqInt, (((usqInt)-1 /* minimumUnusedHeadroom */ << 3) | 1)));
    longAtput(
        (void *)((resultSqInt + BaseHeaderSize) + (75U << (shiftForWord()))),
        (((usqInt)-1 /* minimumUnusedHeadroom */ << 3) | 1));
    beRootIfOld(resultSqInt);

    /* begin methodReturnValue: */
    assert(!((failed())));
    longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
              resultSqInt);
    stackPointer = sp;
    return;
  }
  if (argumentCount > 2) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadNumArgs;
    return;
  }

  /* index read & checks */
  index =
      longAt(stackPointer + (((argumentCount == 1 ? 0 : 1)) * BytesPerWord));
  if (!((((index) & 7) == 1))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  index = (index >> 3);
  if ((index < 1) || (index > paramsArraySize)) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    return;
  }
  if (argumentCount == 1) {
    /* begin primitiveGetVMParameter: */
    switch (index) {
    case 1:
      result = positive64BitIntegerFor(
          (/* begin oldSpaceSize */ /* begin totalOldSpaceSize */
           assert((totalBytesInSegments()) == totalHeapSizeIncludingBridges),
           totalHeapSizeIncludingBridges));
      goto l1;
      break;
    case 2:
      result = (((usqInt)((freeStart - (((eden).start))) +
                          (pastSpaceStart - (((pastSpace).start))))
                 << 3) |
                1);
      goto l1;
      break;
    case 3:
      result = positive64BitIntegerFor(
          (newSpaceCapacity()) +
          ((/* begin totalOldSpaceSize */
            assert((totalBytesInSegments()) == totalHeapSizeIncludingBridges),
            totalHeapSizeIncludingBridges)));
      goto l1;
      break;
    case 6:
      result =
          (((usqInt)(((sqInt)(((scavengerTenuringThreshold()) *
                               (((pastSpace.limit)) - ((pastSpace.start)))) /
                              (8 * BytesPerOop))))
            << 3) |
           1);
      goto l1;
      break;
    case 7:
      result = (((usqInt)statFullGCs << 3) | 1);
      goto l1;
      break;
    case 8:
      result = ((((statFullGCUsecs + 500) / 1000) << 3) | 1);
      goto l1;
      break;
    case 9:
      result = (((usqInt)statScavenges << 3) | 1);
      goto l1;
      break;
    case 10:
      result = ((((statScavengeGCUsecs + 500) / 1000) << 3) | 1);
      goto l1;
      break;
    case 11:
      result = (((usqInt)statTenures << 3) | 1);
      goto l1;
      break;
    case 12:
      result = (((usqInt)eventTraceMask << 3) | 1);
      goto l1;
      break;
    case 13:
      result =
#if VM_TICKER
          positive64BitIntegerFor(ioVMTickerStartUSecs())
#else
          ConstZero
#endif
          ;
      goto l1;
      break;
    case 14:
      result =
#if VM_TICKER
          positive64BitIntegerFor(ioVMTickerCount())
#else
          ConstZero
#endif
          ;
      goto l1;
      break;
    case 15:
      result =
#if VM_TICKER
          positive64BitIntegerFor(ioVMTickeeCallCount())
#else
          ConstZero
#endif
          ;
      goto l1;
      break;
    case 16:
      result = positive64BitIntegerFor(statIdleUsecs);
      goto l1;
      break;
    case 17:
      result = ConstZero;
      goto l1;
      break;
    case 18:
      result = ((((statCompactionUsecs + 500) / 1000) << 3) | 1);
      goto l1;
      break;
    case 19:
      result = (((usqInt)(scavengeThreshold - (((eden).start))) << 3) | 1);
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
      result = (((usqInt)statRootTableOverflows << 3) | 1);
      goto l1;
      break;
    case 23:
      result = (((usqInt)extraVMMemory << 3) | 1);
      goto l1;
      break;
    case 24:
      result = (((usqInt)shrinkThreshold << 3) | 1);
      goto l1;
      break;
    case 25:
      result = (((usqInt)growHeadroom << 3) | 1);
      goto l1;
      break;
    case 26:
      result = (((usqInt)(ioHeartbeatMilliseconds()) << 3) | 1);
      goto l1;
      break;
    case 27:
      result = (((usqInt)statMarkCount << 3) | 1);
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
      result = (((usqInt)statCompactPassCount << 3) | 1);
      goto l1;
      break;
    case 0x1F:
      result = (((usqInt)statGrowMemory << 3) | 1);
      goto l1;
      break;
    case 32:
      result = (((usqInt)statShrinkMemory << 3) | 1);
      goto l1;
      break;
    case 33:
      result = (((usqInt)statRootTableCount << 3) | 1);
      goto l1;
      break;
    case 34:
      result = positive64BitIntegerFor(currentAllocatedBytes());
      goto l1;
      break;
    case 35:
      result = (((usqInt)statSurvivorCount << 3) | 1);
      goto l1;
      break;
    case 36:
      result = ((((statGCEndUsecs / 1000) & MillisecondClockMask) << 3) | 1);
      goto l1;
      break;
    case 37:
      result = (((usqInt)0 /* statSpecialMarkCount */ << 3) | 1);
      goto l1;
      break;
    case 38:
      result = ((((statIGCDeltaUsecs + 500) / 1000) << 3) | 1);
      goto l1;
      break;
    case 39:
      result = (((usqInt)statPendingFinalizationSignals << 3) | 1);
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
      result = (((usqInt)numStackPages << 3) | 1);
      goto l1;
      break;
    case 43:
      result = (((usqInt)desiredNumStackPages << 3) | 1);
      goto l1;
      break;
    case 44:
      result = (((usqInt)(((eden.limit)) - ((eden.start))) << 3) | 1);
      goto l1;
      break;
    case 45:
      result = (((usqInt)desiredEdenBytes << 3) | 1);
      goto l1;
      break;
    case 46:
      result = nilObj;
      goto l1;
      break;
    case 47:
      result = nilObj;
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
      result = nilObj;
      goto l1;
      break;
    case 51:
      result = (((usqInt)(((oldImageBaseAddress != oldSpaceStart ? 1 : 0)) +
                          ((sqImageFileIsEmbedded() ? 2 : 0)))
                 << 3) |
                1);
      goto l1;
      break;
    case 52:
      result = (((usqInt)(rootTableCapacity()) << 3) | 1);
      goto l1;
      break;
    case 53:
      result = (((usqInt)numSegments << 3) | 1);
      goto l1;
      break;
    case 54:
      result = ((totalFreeOldSpace << 3) | 1);
      goto l1;
      break;
    case 55:
      result = floatObjectOf(getHeapGrowthToSizeGCRatio());
      goto l1;
      break;
    case 56:
      result = positive64BitIntegerFor(statProcessSwitch);
      goto l1;
      break;
    case 57:
      result = positive64BitIntegerFor(statIOProcessEvents);
      goto l1;
      break;
    case 58:
      result = positive64BitIntegerFor(statForceInterruptCheck);
      goto l1;
      break;
    case 59:
      result = positive64BitIntegerFor(statCheckForEvents);
      goto l1;
      break;
    case 60:
      result = positive64BitIntegerFor(statStackOverflow);
      goto l1;
      break;
    case 61:
      result = positive64BitIntegerFor(statStackPageDivorce);
      goto l1;
      break;
    case 0x3E:
      result = nilObj;
      goto l1;
      break;
    case 0x3F:
      result = nilObj;
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
      result = (((usqInt)bytesPerPage << 3) | 1);
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
      result = (((usqInt)(statMaxPageCountWhenMapping) << 3) | 1);
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
      result = ((((statMarkUsecs + 500) / 1000) << 3) | 1);
      goto l1;
      break;
    case 73:
      result = ((((statSweepUsecs + 500) / 1000) << 3) | 1);
      goto l1;
      break;
    case 74:
      result = (((usqInt)((statMaxAllocSegmentTime + 500) / 1000) << 3) | 1);
      goto l1;
      break;
    case 75:
      result = /* booleanObjectOf: */
          (primitiveDoMixedArithmetic ? trueObj : falseObj);
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
    oop = (!(result) ? nilObj : result);

    /* begin methodReturnValue: */
    assert(!((failed())));
    longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
              oop);
    stackPointer = sp;
    return;
  }

  /* read VM parameter; written this way to avoid branch limits in V3 bytecode
     set written thus to enable Slang inlining of primitiveGetVMParameter: write
     a VM parameter */
  argOop = longAt(stackPointer);

  /* begin primitiveSetVMParameter:arg: */
  switch (index) {
  case 17:
  case 55:
  case 68:
    if (!((/* isFloatInstance: */
           ((tagBits = argOop & (tagMask()))
                ? tagBits == (smallFloatTag())
                : ((longAt((void *)(argOop))) & (classIndexMask())) ==
                      ClassFloatCompactIndex)) ||
          ((((argOop) & 7) == 1)))) {
      primFailCode = PrimErrBadArgument;
    }
    break;
  case 67:
    /* begin positiveMachineIntegerValueOf: */
    if ((((argOop) & 7) == 1)) {
      value = (argOop >> 3);
      if (value < 0) {
        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
        arg = ((usqIntptr_t)null);
        goto l2;
      }
      arg = ((usqIntptr_t)value);
      goto l2;
    }

    /* don't inline the rare case */
    arg = positiveMachineIntegerValueOfObj(argOop);
    /* end positiveMachineIntegerValueOf: */
  l2:;
    break;
  case 75:
    /* begin booleanValueOf: */
    if (argOop == trueObj) {
      arg = 1;
      goto l3;
    }
    if (argOop == falseObj) {
      arg = 0;
      goto l3;
    }

    /* begin success: */
    if (!primFailCode) {
      primFailCode = 1;
    }
    arg = null;
    /* end booleanValueOf: */
  l3:;
    break;
  default:
    arg = (argOop >> 3);
  }
  if (primFailCode) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    goto l6;
  }

  /* assume failure, then set success for handled indices */

  /* primitiveFailFor: */
  primFailCode = PrimErrBadArgument;
  switch (index) {
  case 5:
    break;
  case 6:
    resultSqInt =
        (((usqInt)(((sqInt)(((scavengerTenuringThreshold()) *
                             (((pastSpace.limit)) - ((pastSpace.start)))) /
                            (8 * BytesPerOop))))
          << 3) |
         1);

    /* begin tenuringThreshold: */
    if (arg < 0) {
      primFailCode = PrimErrBadArgument;
      goto l4;
    }
    aProportion = (((double)(arg * (8 * BytesPerOop)))) /
                  (((double)(((pastSpace.limit)) - ((pastSpace.start)))));

    /* begin scavengerTenuringThreshold: */
    tenuringProportion = aProportion;
    tenureThreshold =
        (aProportion == 0.0
             ? 0
             : (round(((((pastSpace.limit)) - ((pastSpace.start))) *
                       (1.0 - aProportion)))) +
                   ((pastSpace.start)));
    primFailCode = 0;
    /* end tenuringThreshold: */
  l4:;
    break;
  case 12:
    resultSqInt = (((usqInt)eventTraceMask << 3) | 1);
    eventTraceMask = arg;

    /* begin initPrimCall */
    primFailCode = 0;
    break;
  case 11:
    if (arg >= 0) {
      resultSqInt = (((usqInt)statTenures << 3) | 1);

      /* begin statTenures: */
      statTenures = arg;
      primFailCode = 0;
    }
    break;
  case 17:
    break;
  case 23:
    resultSqInt = (((usqInt)extraVMMemory << 3) | 1);
    extraVMMemory = arg;

    /* begin initPrimCall */
    primFailCode = 0;
    break;
  case 24:
    if (arg > 0) {
      resultSqInt = (((usqInt)shrinkThreshold << 3) | 1);

      /* begin shrinkThreshold: */
      shrinkThreshold = arg;
      primFailCode = 0;
    }
    break;
  case 25:
    if (arg > 0) {
      resultSqInt = (((usqInt)growHeadroom << 3) | 1);

      /* growHeadroom: */
      growHeadroom = arg;

      /* begin initPrimCall */
      primFailCode = 0;
    }
    break;
  case 26:
    if (arg >= 0) {
      resultSqInt = (((usqInt)(ioHeartbeatMilliseconds()) << 3) | 1);
      ioSetHeartbeatMilliseconds(arg);

      /* begin initPrimCall */
      primFailCode = 0;
    }
    break;
  case 34:
    if (arg >= 0) {
      resultSqInt = positive64BitIntegerFor(currentAllocatedBytes());

      /* begin setCurrentAllocatedBytesTo: */
      delta = (currentAllocatedBytes()) - statAllocatedBytes;
      statAllocatedBytes = arg;
      oldSpaceUsePriorToScavenge += delta;
      assert((currentAllocatedBytes()) == arg);

      /* begin initPrimCall */
      primFailCode = 0;
    }
    break;
  case 43:
    if (((arg >= 0) && (arg <= 0xFFFF))) {
      resultSqInt = (((usqInt)desiredNumStackPages << 3) | 1);
      desiredNumStackPages = arg;

      /* begin initPrimCall */
      primFailCode = 0;
    }
    break;
  case 45:
    if (arg >= 0) {
      resultSqInt = (((usqInt)desiredEdenBytes << 3) | 1);
      desiredEdenBytes = arg;

      /* begin initPrimCall */
      primFailCode = 0;
    }
    break;
  case 47:
    break;
  case 48:
    if (arg >= 0) {
      oldPrimitiveDoMixedArithmetic = primitiveDoMixedArithmetic;
      resultSqInt =
          (((usqInt)(((usqInt)((getImageHeaderFlags()))) >> 2) << 3) | 1);

      /* begin initPrimCall */
      primFailCode = 0;
      if ((((usqInt)arg)) > 0x1FF) {
        /* primitiveFailFor: */
        primFailCode = PrimErrUnsupported;
        goto l5;
      }

      /* threadAffinity := flags anyMask: 1. specific to CoInterpreterMT */
      imageHeaderFlags = (((arg & 1) != 0) ? imageHeaderFlags | 4
                                           : ((imageHeaderFlags | 4) - 4));

      /* flagInterpretedMethods := flags anyMask: 2. specific to CoInterpreter
       */
      imageHeaderFlags = (((arg & 2) != 0) ? imageHeaderFlags | 8
                                           : ((imageHeaderFlags | 8) - 8));
      preemptionYields = (!(arg & 4));

      /* noThreadingOfGUIThread := flags anyMask: 8.. specific to
       * CoInterpreterMT */
      imageHeaderFlags = (((arg & 8) != 0) ? imageHeaderFlags | 32
                                           : ((imageHeaderFlags | 32) - 32));
      newFinalization = ((arg & 16) != 0);
      sendWheelEvents = ((arg & 32) != 0);
      primitiveDoMixedArithmetic = (!(arg & 64));
      imageHeaderFlags =
          (((arg & 128) != 0) ? imageHeaderFlags | 0x200
                              : ((imageHeaderFlags | 0x200) - 0x200));
      upscaleDisplayIfHighDPI = (!(arg & 0x100));
      /* end setImageHeaderFlags: */
    l5:
      if ((primFailCode == 0) &&
          (oldPrimitiveDoMixedArithmetic != primitiveDoMixedArithmetic)) {
        /* begin flushMethodCache */
        memset(methodCache, 0, MethodCacheSize * (sizeof(methodCache[0])));

        /* this for primitiveExternalMethod */
        lastMethodCacheProbeWrite = 0;

        /* begin flushAtCache */
        memset(atCache, 0, AtCacheTotalSize * (sizeof(atCache[0])));
        goto l6;
      }
    }
    break;
  case 49:
    if (((arg >= 0) && (arg <= 0xFFFF))) {
      resultSqInt = (((usqInt)(ioGetMaxExtSemTableSize()) << 3) | 1);

      /* begin initPrimCall */
      primFailCode = 0;
      maxExtSemTabSizeSet = 1;
      ioSetMaxExtSemTableSize(arg);
    }
    break;
  case 50:
    break;
  case 55:
    resultSqInt = floatObjectOf(getHeapGrowthToSizeGCRatio());

    /* begin setHeapGrowthToSizeGCRatio: */
    heapGrowthToSizeGCRatio = loadFloatOrIntFrom(argOop);
    primFailCode = 0;
    break;
  case 67:
    if (arg >= 0) {
      resultSqInt = positive64BitIntegerFor(maxOldSpaceSize);
      limit = arg;

      /* begin setMaxOldSpaceSize: */
      maxOldSpaceSize = limit;
      primFailCode = 0;
    }
    break;
  case 68:
    resultSqInt = floatObjectOf(statAverageLivePagesWhenMapping());

    /* begin initPrimCall */
    primFailCode = 0;
    if ((loadFloatOrIntFrom(argOop)) == 0.0) {
      statPageCountWhenMappingSum = (statNumMaps = 0);
    } else {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
    }
    break;
  case 69:
    if (arg >= 0) {
      resultSqInt = (((usqInt)(statMaxPageCountWhenMapping) << 3) | 1);

      /* begin statMaxPageCountWhenMapping: */
      statMaxPageCountWhenMapping = arg;
      primFailCode = 0;
    }
    break;
  case 74:
    if (arg >= 0) {
      resultSqInt =
          (((usqInt)((statMaxAllocSegmentTime + 500) / 1000) << 3) | 1);

      /* begin statMaxAllocSegmentTime: */
      statMaxAllocSegmentTime = arg;
      primFailCode = 0;
    }
    break;
  case 75:
    resultSqInt = /* booleanObjectOf: */
        (primitiveDoMixedArithmetic ? trueObj : falseObj);

    /* begin initPrimCall */
    primFailCode = 0;
    mustFlush = primitiveDoMixedArithmetic != arg;
    primitiveDoMixedArithmetic = arg;
    if (mustFlush) {
      /* begin flushMethodCache */
      memset(methodCache, 0, MethodCacheSize * (sizeof(methodCache[0])));

      /* this for primitiveExternalMethod */
      lastMethodCacheProbeWrite = 0;

      /* begin flushAtCache */
      memset(atCache, 0, AtCacheTotalSize * (sizeof(atCache[0])));
      goto l6;
    }
    break;
  default:;
  }

  /* disable inlining...
     disable inlining...
     disable inlining... */
  if (primFailCode) {
    /* primitiveFailFor: */
    primFailCode = PrimErrInappropriate;
  } else {
    /* begin methodReturnValue: */
    assert(!((failed())));
    longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
              resultSqInt);
    stackPointer = sp;
  }
  /* end primitiveSetVMParameter:arg: */
l6:;
}