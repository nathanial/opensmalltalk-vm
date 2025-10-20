/* Extracted from B2DPlugin.c:8060 (function primitiveGetTimes). */

EXPORT(sqInt)
primitiveGetTimes(void) {
  sqInt failureCode;
  sqInt statOop;
  int *stats;

  if (!((methodArgumentCount()) == 1)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFrom(stackValue(1)))) {
    return primitiveFailFor(failureCode);
  }
  statOop = stackObjectValue(0);
  if (!((!(failed())) &&
        ((isWords(statOop)) && ((slotSizeOf(statOop)) >= 9)))) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  stats = firstIndexableField(statOop);
  stats[0] = ((stats[0]) + (workBuffer[GWTimeInitializing]));
  stats[1] = ((stats[1]) + (workBuffer[GWTimeFinishTest]));
  stats[2] = ((stats[2]) + (workBuffer[GWTimeNextGETEntry]));
  stats[3] = ((stats[3]) + (workBuffer[GWTimeAddAETEntry]));
  stats[4] = ((stats[4]) + (workBuffer[GWTimeNextFillEntry]));
  stats[5] = ((stats[5]) + (workBuffer[GWTimeMergeFill]));
  stats[6] = ((stats[6]) + (workBuffer[GWTimeDisplaySpan]));
  stats[7] = ((stats[7]) + (workBuffer[GWTimeNextAETEntry]));
  stats[8] = ((stats[8]) + (workBuffer[GWTimeChangeAETEntry]));
  pop(1);
  return 0;
}