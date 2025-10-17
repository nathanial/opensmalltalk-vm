/* Extracted from interp.c:37939 (function isinstanceOfcompactClassIndex). */

/*	Answer if oop is an instance of the given class. If the class has a
        (non-zero) compactClassIndex use that to speed up the check. N.B.
   Inlining should result in classOop not being accessed if oop's compact class
   index and compactClassIndex are non-zero. */

/* SpurMemoryManager>>#is:instanceOf:compactClassIndex: */

static sqInt isinstanceOfcompactClassIndex(
    sqInt oop, sqInt classOop, sqInt compactClassIndex) {
  sqInt ccIndex;

  if (((oop & (tagMask())) != 0)) {
    return 0;
  }

  /* begin isClassOfNonImm:equalTo:compactClassIndex: */
  assert(!(isImmediate(oop)));
  ccIndex = (longAt((void *)(oop))) & (classIndexMask());
  if (compactClassIndex) {
    return compactClassIndex == ccIndex;
  } else {
    return classOop == (classAtIndex(ccIndex));
  }
}