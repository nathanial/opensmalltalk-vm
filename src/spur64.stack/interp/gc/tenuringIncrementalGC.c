/* Extracted from interp.c:46219 (function tenuringIncrementalGC). */

/*	Do an incremental GC that tenures all surviving young objects to old
        space. The selector tenuringIncrementalGC refers to the V3 incremental
        collector, which is the collector for young objects, equivalent in
        function to the scavenger.
        The Spur incremental collector is called the global incremental garbage
        collector or globalIGC for short.
 */

/* SpurMemoryManager>>#tenuringIncrementalGC */

void tenuringIncrementalGC(void) {
  sqInt savedTenuringThreshold;

  /* begin flushNewSpace */
  savedTenuringThreshold = tenureThreshold;
  tenureThreshold = oldSpaceStart;
  scavengingGCTenuringIf(TenureByAge);

  /* begin setRawTenuringThreshold: */
  tenureThreshold = savedTenuringThreshold;
  assert((rememberedSetSize) == 0);
  assert(pastSpaceStart == (((pastSpace).start)));
  assert(freeStart == (((eden).start)));
}