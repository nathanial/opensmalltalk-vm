/* Extracted from interp.c:34131 (function doScavenge). */

/*	The inner shell for scavenge, abstrascted out so globalGarbageCollect
   can use it.
 */

/* SpurMemoryManager>>#doScavenge: */

static void doScavenge(sqInt tenuringCriterion) {
  float fractionSurvived;
  sqInt oldStart;
  SpurNewSpaceSpace temp;
  usqInt use;

  /* begin doAllocationAccountingForScavenge */
  /* begin currentAllocatedBytes */
  use =
      ((assert((totalBytesInSegments()) == totalHeapSizeIncludingBridges),
        totalHeapSizeIncludingBridges - (numSegments * (2 * BaseHeaderSize)))) -
      totalFreeOldSpace;
  statAllocatedBytes = (statAllocatedBytes + (freeStart - (((eden).start)))) +
                       (use - oldSpaceUsePriorToScavenge);
  gcPhaseInProgress = ScavengeInProgress;

  /* begin scavenge: */
  statSurvivorCount = 0;
  tenureCriterion = tenuringCriterion;

  /* begin logStartScavenge */
  (scavengeLogRecord.sEdenBytes = freeStart - ((eden.start)));
  (scavengeLogRecord.sPastBytes = pastSpaceStart - ((pastSpace.start)));
  (scavengeLogRecord.sRememberedSetSize = rememberedSetSize);
  (scavengeLogRecord.sRememberedSetRedZone = rememberedSetRedZone);
  (scavengeLogRecord.sRememberedSetLimit = rememberedSetLimit);
  (scavengeLogRecord.sStatTenures = statTenures);

  /* begin strategizeToLimitRememberedTable */
  if ((tenureCriterion == TenureByAge) &&
      (rememberedSetSize >= rememberedSetRedZone)) {
    tenureCriterion = TenureToShrinkRT;
    computeRefCountToShrinkRT();
  }

  /* begin logTenuringPolicy */
  (scavengeLogRecord.tTenureCriterion = tenureCriterion);
  (scavengeLogRecord.tTenureThreshold =
       ((tenureCriterion == TenureByAge) &&
                (tenureThreshold > ((pastSpace.start)))
            ? tenureThreshold - ((pastSpace.start))
            : 0));
  (scavengeLogRecord.tRefCountToShrinkRT = refCountToShrinkRT);
  scavengeLoop();
  processWeaklings();

  /* begin computeTenuringThreshold */
  fractionSurvived =
      (((futureSpace.limit)) == ((futureSpace.start))
           ? 0.0
           : (((double)(futureSurvivorStart - ((futureSpace.start))))) /
                 (((futureSpace.limit)) - ((futureSpace.start))));
  tenureThreshold = (fractionSurvived > 0.9
                         ? (round(((((pastSpace.limit)) - ((pastSpace.start))) *
                                   (1.0 - tenuringProportion)))) +
                               ((pastSpace.start))
                         : 0);

  /* begin exchangeSurvivorSpaces */
  temp = pastSpace;
  pastSpace = futureSpace;
  futureSpace = temp;

  /* begin logEndScavenge */
  (scavengeLogRecord.eSurvivorBytes =
       futureSurvivorStart - ((pastSpace.start)));
  (scavengeLogRecord.eRememberedSetSize = rememberedSetSize);
  (scavengeLogRecord.eStatTenures = statTenures);

  /* begin initFutureSpaceStart */
  oldStart = futureSurvivorStart;
  futureSurvivorStart = (futureSpace.start);
  pastSpaceStart = oldStart;
  assert(oopisGreaterThanOrEqualToandLessThanOrEqualTo(
      pastSpaceStart, ((pastSpace).start), ((pastSpace).limit)));
  freeStart = ((eden).start);
  gcPhaseInProgress = 0;

  /* begin resetAllocationAccountingAfterGC */
  oldSpaceUsePriorToScavenge =
      ((assert((totalBytesInSegments()) == totalHeapSizeIncludingBridges),
        totalHeapSizeIncludingBridges - (numSegments * (2 * BaseHeaderSize)))) -
      totalFreeOldSpace;
}