/* Extracted from interp.c:54952 (function isBaseFrame). */

/*	A base frame (first frame in a stack page) is so marked by having a null
        saved fp.
 */

/* StackInterpreter>>#isBaseFrame: */

static int isBaseFrame(char *theFP) {
  return (longAt(theFP + FoxSavedFP)) == 0;
}