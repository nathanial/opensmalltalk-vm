/* Extracted from interp.c:34131 (function doScavenge). */

/*	The inner shell for scavenge, abstrascted out so globalGarbageCollect can
	use it.
 */

	/* SpurMemoryManager>>#doScavenge: */

static NoDbgRegParms void
doScavenge(sqInt tenuringCriterion)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    float fractionSurvived;
    sqInt oldStart;
    SpurNewSpaceSpace temp;
    usqInt use;

	/* begin doAllocationAccountingForScavenge */
	/* begin currentAllocatedBytes */
	use = ((assert((totalBytesInSegments()) == GIV(totalHeapSizeIncludingBridges)),
GIV(totalHeapSizeIncludingBridges) - (GIV(numSegments) * (2 * BaseHeaderSize)))) - GIV(totalFreeOldSpace);
	GIV(statAllocatedBytes) = (GIV(statAllocatedBytes) + (GIV(freeStart) - (((GIV(eden)).start)))) + (use - GIV(oldSpaceUsePriorToScavenge));
	GIV(gcPhaseInProgress) = ScavengeInProgress;

	/* begin scavenge: */
	GIV(statSurvivorCount) = 0;
	GIV(tenureCriterion) = tenuringCriterion;

	/* begin logStartScavenge */
	(GIV(scavengeLogRecord).sEdenBytes = GIV(freeStart) - ((GIV(eden).start)));
	(GIV(scavengeLogRecord).sPastBytes = GIV(pastSpaceStart) - ((GIV(pastSpace).start)));
	(GIV(scavengeLogRecord).sRememberedSetSize = GIV(rememberedSetSize));
	(GIV(scavengeLogRecord).sRememberedSetRedZone = GIV(rememberedSetRedZone));
	(GIV(scavengeLogRecord).sRememberedSetLimit = GIV(rememberedSetLimit));
	(GIV(scavengeLogRecord).sStatTenures = GIV(statTenures));

	/* begin strategizeToLimitRememberedTable */
	if ((GIV(tenureCriterion) == TenureByAge)
	 && (GIV(rememberedSetSize) >= GIV(rememberedSetRedZone))) {
		GIV(tenureCriterion) = TenureToShrinkRT;
		computeRefCountToShrinkRT();
	}

	/* begin logTenuringPolicy */
	(GIV(scavengeLogRecord).tTenureCriterion = GIV(tenureCriterion));
	(GIV(scavengeLogRecord).tTenureThreshold = ((GIV(tenureCriterion) == TenureByAge)
	 && (GIV(tenureThreshold) > ((GIV(pastSpace).start)))
			? GIV(tenureThreshold) - ((GIV(pastSpace).start))
			: 0));
	(GIV(scavengeLogRecord).tRefCountToShrinkRT = GIV(refCountToShrinkRT));
	scavengeLoop();
	processWeaklings();

	/* begin computeTenuringThreshold */
	fractionSurvived = (((GIV(futureSpace).limit)) == ((GIV(futureSpace).start))
				? 0.0
				: (((double) (GIV(futureSurvivorStart) - ((GIV(futureSpace).start))) )) / (((GIV(futureSpace).limit)) - ((GIV(futureSpace).start))));
	GIV(tenureThreshold) = (fractionSurvived > 0.9
				? (round(((((GIV(pastSpace).limit)) - ((GIV(pastSpace).start))) * (1.0 - GIV(tenuringProportion))))) + ((GIV(pastSpace).start))
				: 0);

	/* begin exchangeSurvivorSpaces */
	temp = GIV(pastSpace);
	GIV(pastSpace) = GIV(futureSpace);
	GIV(futureSpace) = temp;

	/* begin logEndScavenge */
	(GIV(scavengeLogRecord).eSurvivorBytes = GIV(futureSurvivorStart) - ((GIV(pastSpace).start)));
	(GIV(scavengeLogRecord).eRememberedSetSize = GIV(rememberedSetSize));
	(GIV(scavengeLogRecord).eStatTenures = GIV(statTenures));

	/* begin initFutureSpaceStart */
	oldStart = GIV(futureSurvivorStart);
	GIV(futureSurvivorStart) = (GIV(futureSpace).start);
	GIV(pastSpaceStart) = oldStart;
	assert(oopisGreaterThanOrEqualToandLessThanOrEqualTo(GIV(pastSpaceStart), ((GIV(pastSpace)).start), ((GIV(pastSpace)).limit)));
	GIV(freeStart) = ((GIV(eden)).start);
	GIV(gcPhaseInProgress) = 0;

	/* begin resetAllocationAccountingAfterGC */
	GIV(oldSpaceUsePriorToScavenge) = ((assert((totalBytesInSegments()) == GIV(totalHeapSizeIncludingBridges)),
GIV(totalHeapSizeIncludingBridges) - (GIV(numSegments) * (2 * BaseHeaderSize)))) - GIV(totalFreeOldSpace);
}