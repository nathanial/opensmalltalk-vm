/* Extracted from interp.c:37258 (function isEphemeronFormat). */

/* SpurMemoryManager>>#isEphemeronFormat: */

static NoDbgRegParms int isEphemeronFormat(sqInt format) {
  return format == (ephemeronFormat());
}