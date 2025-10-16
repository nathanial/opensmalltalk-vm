/* Extracted from interp.c:35474 (function
 * fixFollowedFieldofObjectwithInitialValue). */

/*	Private helper for followField:ofObject: to avoid code duplication for
        rare case.
 */

/* SpurMemoryManager>>#fixFollowedField:ofObject:withInitialValue: */

static NeverInline sqInt fixFollowedFieldofObjectwithInitialValue(
    sqInt fieldIndex, sqInt anObject, sqInt initialValue) {
  sqInt objOop;

  assert(isOopForwarded(initialValue));

  /* inlined followForwarded: for speed (one less test) */
  objOop = initialValue;
  do {
    objOop = fetchPointerofObject(0U, objOop);
  } while (isOopForwarded(objOop));

  storePointerofObjectwithValue(fieldIndex, anObject, objOop);
  return objOop;
}