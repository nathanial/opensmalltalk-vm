/* Extracted from interp.c:38759 (function longPrintInstancesOf). */

/*	Scan the heap printing the oops of any and all objects that are
   instances of aClassOop
 */
/*	useful for VM debugging */

/* SpurMemoryManager>>#longPrintInstancesOf: */

void longPrintInstancesOf(sqInt aClassOop) {
  sqInt classIndex;

  classIndex =
      (long32At((void *)(aClassOop + 4))) & (identityHashHalfWordMask());
  if (classIndex != (isFreeObjectClassIndexPun())) {
    longPrintInstancesWithClassIndex(classIndex);
  }
}