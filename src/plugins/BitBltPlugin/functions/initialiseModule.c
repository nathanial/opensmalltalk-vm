/* Extracted from BitBltPlugin.c:2986 (function initialiseModule). */

EXPORT(sqInt)
initialiseModule(void) {
  initBBOpTable();
  initDither8Lookup();
#if ENABLE_FAST_BLT
  initialiseCopyBits();
#endif

  return 1;
}