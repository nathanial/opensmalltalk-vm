/* Extracted from interp.c:35696 (function followObjFieldofObject). */

/*	Make sure the obj at fieldIndex in anObject is not forwarded (follow the
        forwarder there-in if so). Answer the (possibly followed) obj at
        fieldIndex.  */

/* SpurMemoryManager>>#followObjField:ofObject: */

static sqInt followObjFieldofObject(sqInt fieldIndex, sqInt anObject) {
  sqInt objOop;

  objOop = fetchPointerofObject(fieldIndex, anObject);
  assert(isNonImmediate(objOop));
  if ((!((longAt((void *)(objOop))) &
         ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
    objOop =
        fixFollowedFieldofObjectwithInitialValue(fieldIndex, anObject, objOop);
  }
  return objOop;
}