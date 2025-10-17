/* Extracted from interp.c:40752 (function minSlotsForShortening). */

/*	Answer the minimum number of additional slots to allocate in an object
   to always be able to shorten it. This is enough slots to allocate a
   minimum-sized object. */

/* SpurMemoryManager>>#minSlotsForShortening */

sqInt minSlotsForShortening(void) {
  return 16 /* (allocationUnit * 2) */ / BytesPerOop;
}