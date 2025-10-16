/* Extracted from BitBltPlugin.c:2986 (function initialiseModule). */
/* BitBltSimulation>>#initialiseModule */
/*	Load BitBlt from the oop.
        This function is exported for the Balloon engine. */

EXPORT(sqInt)
initialiseModule(void) {
  initBBOpTable();
  initDither8Lookup();
  return 1;
}
