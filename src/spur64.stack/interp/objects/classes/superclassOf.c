/* Extracted from interp.c:65014 (function superclassOf). */

/*	Using a read barrier here simplifies the become implementation and costs
        very little because the class index and superclass almost certainly
   share a cache line. */

/* StackInterpreter>>#superclassOf: */

sqInt superclassOf(sqInt classPointer) {
  sqInt objOop;

  objOop = followObjFieldofObject(SuperclassIndex, classPointer);
  return objOop;
}