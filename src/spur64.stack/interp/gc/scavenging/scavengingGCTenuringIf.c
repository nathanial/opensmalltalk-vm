/* Extracted from interp.c:45340 (function scavengingGCTenuringIf). */

/*	Run the scavenger. */

/* SpurMemoryManager>>#scavengingGCTenuringIf: */

static void scavengingGCTenuringIf(sqInt tenuringCriterion) {
  sqInt i;
  sqInt n;
  sqInt probe;

  assert(remapBufferCount == 0);
  if (!(asserta(((((eden).limit)) - freeStart) >
                (interpreterAllocationReserveBytes())))) {
    /* begin tab */
    printChar('\t');
    n = (((eden).limit)) - freeStart;

    /* begin printNum: */
    fprintf(transcript, "%" PRIdSQINT "", ((sqInt)n));
    printChar(' ');
    n = interpreterAllocationReserveBytes();

    /* begin printNum: */
    fprintf(transcript, "%" PRIdSQINT "", ((sqInt)n));
    printChar(' ');
    n = (interpreterAllocationReserveBytes()) - ((((eden).limit)) - freeStart);

    /* begin printNum: */
    fprintf(transcript, "%" PRIdSQINT "", ((sqInt)n));
    cr();
  }

  /* begin checkMemoryMap */
  assert(isYoungObject(newSpaceStart));
  assert(!(isOldObject(newSpaceStart)));
  assert(isYoungObject(oldSpaceStart - BytesPerWord));
  assert(!(isOldObject(oldSpaceStart - BytesPerWord)));
  assert(!(isYoungObject(oldSpaceStart)));
  assert(isOldObject(oldSpaceStart));
  assert(!(isYoungObject(endOfMemory)));
  assert(isOldObject(endOfMemory));

  /* begin checkFreeSpace: */
  assert(bitsSetInFreeSpaceMaskForAllFreeLists());
  assert(totalFreeOldSpace == (totalFreeListBytes()));
  if (((checkForLeaks & (GCCheckFreeSpace | GCModeNewSpace)) ==
       (GCCheckFreeSpace | GCModeNewSpace))) {
    runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
  }
  runLeakCheckerFor(GCModeNewSpace);

  /* begin preGCAction: */
  if (stackPage) {
    externalWriteBackHeadFramePointers();
  }

  /* begin flushMethodCacheFrom:to: */
  probe = 0;
  for (i = 1; i <= MethodCacheEntries; i += 1) {
    if (methodCache[probe + MethodCacheSelector]) {
      if (((oopisGreaterThanOrEqualTo(methodCache[probe + MethodCacheSelector],
                                      newSpaceStart)) &&
           (oopisLessThan(methodCache[probe + MethodCacheSelector],
                          oldSpaceStart))) ||
          ((oopisGreaterThanOrEqualTo(methodCache[probe + MethodCacheMethod],
                                      newSpaceStart)) &&
           (oopisLessThan(methodCache[probe + MethodCacheMethod],
                          oldSpaceStart)))) {
        methodCache[probe + MethodCacheSelector] = 0;
      }
    }
    probe += MethodCacheEntrySize;
  }

  /* begin flushAtCache */
  memset(atCache, 0, AtCacheTotalSize * (sizeof(atCache[0])));

  /* would prefer this to be in mapInterpreterOops, but
     compatibility with ObjectMemory dictates it goes here. */
  needGCFlag = 0;
  gcStartUsecs = ioUTCMicrosecondsNow();
  doScavenge(tenuringCriterion);
  statScavenges += 1;
  statGCEndUsecs = ioUTCMicrosecondsNow();
  statSGCDeltaUsecs = statGCEndUsecs - gcStartUsecs;
  statScavengeGCUsecs += statSGCDeltaUsecs;
  statRootTableCount = rememberedSetSize;

  /* begin logScavenge */
  if (scavengeLog) {
    writeScavengeLog();
  }

  /* begin postGCAction: */
  signalSemaphoreWithIndex(gcSemaphoreIndex);
  postGCUpdateDisplayBits();
  runLeakCheckerFor(GCModeNewSpace);

  /* begin checkFreeSpace: */
  assert(bitsSetInFreeSpaceMaskForAllFreeLists());
  assert(totalFreeOldSpace == (totalFreeListBytes()));
  if (((checkForLeaks & (GCCheckFreeSpace | GCModeNewSpace)) ==
       (GCCheckFreeSpace | GCModeNewSpace))) {
    runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
  }
}