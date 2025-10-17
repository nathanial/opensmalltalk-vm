/* Extracted from interp.c:40956 (function noFixupFollowFieldofObject). */

/*	Make sure the oop at fieldIndex in anObject is not forwarded (follow the
        forwarder there-in if so). Answer the (possibly followed) oop at
        fieldIndex.  */

/* SpurMemoryManager>>#noFixupFollowField:ofObject: */

static sqInt noFixupFollowFieldofObject(sqInt fieldIndex, sqInt anObject) {
  sqInt objOop;
  sqInt referent;

  objOop = fetchPointerofObject(fieldIndex, anObject);
  if (isOopForwarded(objOop)) {
    objOop = followForwarded(objOop);
  }
  return objOop;
}