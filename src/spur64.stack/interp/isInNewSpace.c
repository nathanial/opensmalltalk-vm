/* Extracted from interp.c:37403 (function isInNewSpace). */

/* SpurMemoryManager>>#isInNewSpace: */

static NoDbgRegParms sqInt isInNewSpace(sqInt objOop) {
  return (oopisLessThan(objOop, oldSpaceStart)) &&
         (oopisGreaterThanOrEqualTo(objOop, newSpaceStart));
}