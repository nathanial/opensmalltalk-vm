/* Extracted from interp.c:37258 (function isEphemeronFormat). */

/* SpurMemoryManager>>#isEphemeronFormat: */

static int isEphemeronFormat(sqInt format) {
  return format == (ephemeronFormat());
}