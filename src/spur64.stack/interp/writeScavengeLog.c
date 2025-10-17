/* Extracted from interp.c:29472 (function writeScavengeLog). */

/*	Output the entire record. */

	/* SpurGenerationScavenger>>#writeScavengeLog */

static NeverInline void
writeScavengeLog(void)
{
    static char *policyNames[] = {"", "by age", "by class", "to shrink rt", "don't tenure", "mark on tenure"};

	/* log data collected by logStartScavenge */
	fprintf(scavengeLog,
			"scavenge %" PRIdSQINT " eden bytes: 0x%" PRIxSQINT "/%" PRIdSQINT " past bytes: 0x%" PRIxSQINT "/%" PRIdSQINT "\n\trem set: %" PRIdSQINT " redzone: %" PRIdSQINT " size: %" PRIdSQINT "\n",
			statScavenges,
			(scavengeLogRecord.sEdenBytes),
			(scavengeLogRecord.sEdenBytes),
			(scavengeLogRecord.sPastBytes),
			(scavengeLogRecord.sPastBytes),
			(scavengeLogRecord.sRememberedSetSize),
			(scavengeLogRecord.sRememberedSetRedZone),
			(scavengeLogRecord.sRememberedSetLimit));

	/* log data collected by logTenuringPolicy */
	fprintf(scavengeLog,
			(((scavengeLogRecord.tTenureCriterion)) == TenureToShrinkRT
			? "\ttenure below 0x%" PRIxSQINT "/%" PRIdSQINT " %s refct %" PRIdSQINT "\n"
			: "	tenure below 0x%lx/%ld %s\n"),
			(scavengeLogRecord.tTenureThreshold),
			(scavengeLogRecord.tTenureThreshold),
			policyNames[(scavengeLogRecord.tTenureCriterion)],
			(scavengeLogRecord.tRefCountToShrinkRT));

	/* log data collected by logEndScavenge */
	fprintf(scavengeLog,
			"\tsurvivor bytes: 0x%" PRIxSQINT "/%" PRIdSQINT " rem set: %" PRIdSQINT " tenured: %" PRIdSQINT " usecs: %" PRIdSQINT "\n",
			(scavengeLogRecord.eSurvivorBytes),
			(scavengeLogRecord.eSurvivorBytes),
			(scavengeLogRecord.eRememberedSetSize),
			((scavengeLogRecord.eStatTenures)) - ((scavengeLogRecord.sStatTenures)),
			statSGCDeltaUsecs);
	fflush(scavengeLog);
}