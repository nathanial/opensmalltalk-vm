/* Extracted from B2DPlugin.c:7962 (function primitiveGetCounts). */

EXPORT(sqInt)
primitiveGetCounts(void) {
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
  stats[0] = ((stats[0]) + (workBuffer[GWCountInitializing]));
  stats[1] = ((stats[1]) + (workBuffer[GWCountFinishTest]));
  stats[2] = ((stats[2]) + (workBuffer[GWCountNextGETEntry]));
  stats[3] = ((stats[3]) + (workBuffer[GWCountAddAETEntry]));
  stats[4] = ((stats[4]) + (workBuffer[GWCountNextFillEntry]));
  stats[5] = ((stats[5]) + (workBuffer[GWCountMergeFill]));
  stats[6] = ((stats[6]) + (workBuffer[GWCountDisplaySpan]));
  stats[7] = ((stats[7]) + (workBuffer[GWCountNextAETEntry]));
  stats[8] = ((stats[8]) + (workBuffer[GWCountChangeAETEntry]));
  pop(1);
  return 0;
}