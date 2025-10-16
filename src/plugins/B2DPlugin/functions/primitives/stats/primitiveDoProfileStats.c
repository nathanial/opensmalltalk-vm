/* Extracted from B2DPlugin.c:7779 (function primitiveDoProfileStats). */

EXPORT(sqInt)
primitiveDoProfileStats(void) {
  sqInt newValue;
  int oldValue;

  oldValue = doProfileStats;
  newValue = stackObjectValue(0);
  newValue = booleanValueOf(newValue);
  if (!(failed())) {
    doProfileStats = newValue;
    pop(2);
    pushBool(oldValue);
  }
  return 0;
}