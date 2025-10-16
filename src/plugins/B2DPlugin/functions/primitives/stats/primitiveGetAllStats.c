/* Extracted from B2DPlugin.c:7850 (function primitiveGetAllStats). */

EXPORT(sqInt)
primitiveGetAllStats(void) {
  sqInt countsOop;
  sqInt failureCode;
  int *stats;
  sqInt statsOop;
  sqInt timesOop;

  countsOop = 0;
  statsOop = 0;
  timesOop = 0;
  if (!((methodArgumentCount()) == 3)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFrom(stackValue(3)))) {
    return primitiveFailFor(failureCode);
  }
  if (!((isWords((countsOop = stackValue(2)))) &&
        (((slotSizeOf(countsOop)) >= 9) &&
         ((isWords((timesOop = stackValue(1)))) &&
          (((slotSizeOf(timesOop)) >= 9) &&
           ((isWords((statsOop = stackValue(0)))) &&
            ((slotSizeOf(statsOop)) >= 4))))))) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  stats = firstIndexableField(countsOop);
  stats[0] = ((stats[0]) + (workBuffer[GWCountInitializing]));
  stats[1] = ((stats[1]) + (workBuffer[GWCountFinishTest]));
  stats[2] = ((stats[2]) + (workBuffer[GWCountNextGETEntry]));
  stats[3] = ((stats[3]) + (workBuffer[GWCountAddAETEntry]));
  stats[4] = ((stats[4]) + (workBuffer[GWCountNextFillEntry]));
  stats[5] = ((stats[5]) + (workBuffer[GWCountMergeFill]));
  stats[6] = ((stats[6]) + (workBuffer[GWCountDisplaySpan]));
  stats[7] = ((stats[7]) + (workBuffer[GWCountNextAETEntry]));
  stats[8] = ((stats[8]) + (workBuffer[GWCountChangeAETEntry]));
  stats = firstIndexableField(timesOop);
  stats[0] = ((stats[0]) + (workBuffer[GWTimeInitializing]));
  stats[1] = ((stats[1]) + (workBuffer[GWTimeFinishTest]));
  stats[2] = ((stats[2]) + (workBuffer[GWTimeNextGETEntry]));
  stats[3] = ((stats[3]) + (workBuffer[GWTimeAddAETEntry]));
  stats[4] = ((stats[4]) + (workBuffer[GWTimeNextFillEntry]));
  stats[5] = ((stats[5]) + (workBuffer[GWTimeMergeFill]));
  stats[6] = ((stats[6]) + (workBuffer[GWTimeDisplaySpan]));
  stats[7] = ((stats[7]) + (workBuffer[GWTimeNextAETEntry]));
  stats[8] = ((stats[8]) + (workBuffer[GWTimeChangeAETEntry]));
  stats = firstIndexableField(statsOop);
  stats[0] = ((stats[0]) + (workBuffer[GWBezierMonotonSubdivisions]));
  stats[1] = ((stats[1]) + (workBuffer[GWBezierHeightSubdivisions]));
  stats[2] = ((stats[2]) + (workBuffer[GWBezierOverflowSubdivisions]));
  stats[3] = ((stats[3]) + (workBuffer[GWBezierLineConversions]));
  methodReturnReceiver();
  return 0;
}