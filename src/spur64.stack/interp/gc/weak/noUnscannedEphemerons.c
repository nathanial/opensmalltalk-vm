/* Extracted from interp.c:41032 (function noUnscannedEphemerons). */

/* SpurMemoryManager>>#noUnscannedEphemerons */

static int noUnscannedEphemerons(void) {
  return ((unscannedEphemerons.top)) < ((unscannedEphemerons.start));
}