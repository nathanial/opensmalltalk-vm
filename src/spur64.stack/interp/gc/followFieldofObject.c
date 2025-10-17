/* Extracted from interp.c:35514 (function followFieldofObject). */

/*	Make sure the oop at fieldIndex in anObject is not forwarded (follow the
        forwarder there-in if so). Answer the (possibly followed) oop at
        fieldIndex.  */

/* SpurMemoryManager>>#followField:ofObject: */

static sqInt followFieldofObject(sqInt fieldIndex,
                                               sqInt anObject) {
  sqInt objOop;

  objOop = longAt((void *)((anObject + BaseHeaderSize) +
                           ((((usqInt)(fieldIndex) << (shiftForWord()))))));
  if (/* isOopForwarded: */
      isOopForwarded(objOop)) {
    objOop =
        fixFollowedFieldofObjectwithInitialValue(fieldIndex, anObject, objOop);
  }
  return objOop;
}