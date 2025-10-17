/* Extracted from interp.c:29472 (function writeScavengeLog). */

/*	Output the entire record. */

	/* SpurGenerationScavenger>>#writeScavengeLog */

static NeverInline void
writeScavengeLog(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    static char *policyNames[] = {"", "by age", "by class", "to shrink rt", "don't tenure", "mark on tenure"};

	/* log data collected by logStartScavenge */
	fprintf(GIV(scavengeLog),
			"scavenge %" PRIdSQINT " eden bytes: 0x%" PRIxSQINT "/%" PRIdSQINT " past bytes: 0x%" PRIxSQINT "/%" PRIdSQINT "\n\trem set: %" PRIdSQINT " redzone: %" PRIdSQINT " size: %" PRIdSQINT "\n",
			GIV(statScavenges),
			(GIV(scavengeLogRecord).sEdenBytes),
			(GIV(scavengeLogRecord).sEdenBytes),
			(GIV(scavengeLogRecord).sPastBytes),
			(GIV(scavengeLogRecord).sPastBytes),
			(GIV(scavengeLogRecord).sRememberedSetSize),
			(GIV(scavengeLogRecord).sRememberedSetRedZone),
			(GIV(scavengeLogRecord).sRememberedSetLimit));

	/* log data collected by logTenuringPolicy */
	fprintf(GIV(scavengeLog),
			(((GIV(scavengeLogRecord).tTenureCriterion)) == TenureToShrinkRT
			? "\ttenure below 0x%" PRIxSQINT "/%" PRIdSQINT " %s refct %" PRIdSQINT "\n"
			: "	tenure below 0x%lx/%ld %s\n"),
			(GIV(scavengeLogRecord).tTenureThreshold),
			(GIV(scavengeLogRecord).tTenureThreshold),
			policyNames[(GIV(scavengeLogRecord).tTenureCriterion)],
			(GIV(scavengeLogRecord).tRefCountToShrinkRT));

	/* log data collected by logEndScavenge */
	fprintf(GIV(scavengeLog),
			"\tsurvivor bytes: 0x%" PRIxSQINT "/%" PRIdSQINT " rem set: %" PRIdSQINT " tenured: %" PRIdSQINT " usecs: %" PRIdSQINT "\n",
			(GIV(scavengeLogRecord).eSurvivorBytes),
			(GIV(scavengeLogRecord).eSurvivorBytes),
			(GIV(scavengeLogRecord).eRememberedSetSize),
			((GIV(scavengeLogRecord).eStatTenures)) - ((GIV(scavengeLogRecord).sStatTenures)),
			GIV(statSGCDeltaUsecs));
	fflush(GIV(scavengeLog));
}