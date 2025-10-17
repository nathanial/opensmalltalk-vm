/* Extracted from interp.c:40956 (function noFixupFollowFieldofObject). */

/*	Make sure the oop at fieldIndex in anObject is not forwarded (follow the
        forwarder there-in if so). Answer the (possibly followed) oop at
        fieldIndex.  */

/* SpurMemoryManager>>#noFixupFollowField:ofObject: */

static sqInt noFixupFollowFieldofObject(sqInt fieldIndex, sqInt anObject) {
  sqInt objOop;
  sqInt referent;

  objOop = longAt((void *)((anObject + BaseHeaderSize) +
                           ((((usqInt)(fieldIndex) << (shiftForWord()))))));
  if (isOopForwarded(objOop)) {
    /* begin followForwarded: */
    assert(isUnambiguouslyForwarder(objOop));
    referent =
        longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
    while (isOopForwarded(referent)) {
      referent = longAt(
          (void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
    }
    objOop = referent;
  }
  return objOop;
}