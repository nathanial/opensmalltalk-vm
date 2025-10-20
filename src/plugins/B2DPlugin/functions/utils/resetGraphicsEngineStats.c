/* Extracted from B2DPlugin.c:9518 (function resetGraphicsEngineStats). */

static sqInt resetGraphicsEngineStats(void) {
  workBuffer[GWTimeInitializing] = 0;
  workBuffer[GWTimeFinishTest] = 0;
  workBuffer[GWTimeNextGETEntry] = 0;
  workBuffer[GWTimeAddAETEntry] = 0;
  workBuffer[GWTimeNextFillEntry] = 0;
  workBuffer[GWTimeMergeFill] = 0;
  workBuffer[GWTimeDisplaySpan] = 0;
  workBuffer[GWTimeNextAETEntry] = 0;
  workBuffer[GWTimeChangeAETEntry] = 0;
  workBuffer[GWCountInitializing] = 0;
  workBuffer[GWCountFinishTest] = 0;
  workBuffer[GWCountNextGETEntry] = 0;
  workBuffer[GWCountAddAETEntry] = 0;
  workBuffer[GWCountNextFillEntry] = 0;
  workBuffer[GWCountMergeFill] = 0;
  workBuffer[GWCountDisplaySpan] = 0;
  workBuffer[GWCountNextAETEntry] = 0;
  workBuffer[GWCountChangeAETEntry] = 0;
  workBuffer[GWBezierMonotonSubdivisions] = 0;
  workBuffer[GWBezierHeightSubdivisions] = 0;
  workBuffer[GWBezierOverflowSubdivisions] = 0;
  workBuffer[GWBezierLineConversions] = 0;
  return 0;
}