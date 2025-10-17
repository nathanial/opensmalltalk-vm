/* Extracted from interp.c:37596 (function isReallyYoungObject). */

/*	Answer if obj is young. This for compatibility with SqueakV3 where
        the GC makes all objects young during full GC. Spur doesn't do so. */

/* SpurMemoryManager>>#isReallyYoungObject: */

int isReallyYoungObject(sqInt obj) {
  /* begin isYoungObject: */
  assert(isNonImmediate(obj));
  return oopisLessThan(obj, oldSpaceStart);
}