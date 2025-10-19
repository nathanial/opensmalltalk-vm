/* Extracted from interp.c:18942 (function primitiveLoadImageSegment). */

/*	This primitive is called from Smalltalk as...
        <imageSegment> loadSegmentFrom: aWordArray outPointers: anArray.

        This primitive will load a binary image segment created by
        primitiveStoreImageSegment. It expects the outPointer array to be of the
        proper size, and the wordArray to be well
        formed. It will return as its value the original array of roots, and the
        erstwhile segmentWordArray will have been truncated to a size of one
   word, i.e. retaining the version stamp. If this primitive should fail, the
   segmentWordArray will, sadly, have been reduced to an unrecognizable and
   unusable jumble. But what more could you have done with it anyway? [How about
   saving it so the system functions as primitives are intended? eem 5/9/2017
   16:31]

        In Spur, if the primitive succeeds, the segmentWordArray is also becomed
        into the array of loaded
        objects, to allow fixing up of loaded objects directly without
   nextObject, which Spur doesn't support. */

/* InterpreterPrimitives>>#primitiveLoadImageSegment */

static void primitiveLoadImageSegment(void) {
  sqInt outPointerArray;
  sqInt result;
  sqInt segmentWordArray;

  outPointerArray = longAt(stackPointer);
  segmentWordArray = longAt(stackPointer + (1 * BytesPerWord));

  /* Essential type checks */
  if (!((/* isArray: */
         ((!(outPointerArray & (tagMask())))) &&
         (((byteAt((void *)(outPointerArray + (formatFieldByteOffset())))) &
           (formatMask())) == (arrayFormat()))) &&
        (/* isWords: */
         ((!(segmentWordArray & (tagMask())))) &&
         (((((byteAt((void *)(segmentWordArray + (formatFieldByteOffset())))) &
             (formatMask())) >= (firstLongFormat())) &&
           (((byteAt((void *)(segmentWordArray + (formatFieldByteOffset())))) &
             (formatMask())) <= ((firstShortFormat()) - 1))))))) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }

  /* Must be indexable pointers
     Must be indexable words
     the engine returns the roots array which was first in the segment, or an
     error code on failure. */
  result = loadImageSegmentFromoutPointers(segmentWordArray, outPointerArray);
  if (oopisGreaterThan(result, segmentWordArray)) {
    popthenPush(3, result);
  } else {
    /* primitiveFailFor: */
    primFailCode = result;
  }
}