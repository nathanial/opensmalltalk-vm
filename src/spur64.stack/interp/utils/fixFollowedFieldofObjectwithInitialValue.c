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

  /* begin storePointer:ofObject:withValue: */
  assert(validStorePointerArgs(fieldIndex, anObject, objOop));
  assert(isNonImmediate(anObject));
  if (oopisGreaterThanOrEqualTo(anObject, oldSpaceStart)) {
    if (/* isYoung: */
        ((!(objOop & (tagMask())))) && (oopisLessThan(objOop, oldSpaceStart))) {
      /* begin possibleRootStoreInto: */
      if (!((byteAt((void *)(anObject + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift())))) {
        remember(anObject);
      }
    }
  }

  /* most stores into young objects */
  longAtput((void *)((anObject + BaseHeaderSize) +
                     ((((usqInt)(fieldIndex) << (shiftForWord()))))),
            objOop);
  return objOop;
}