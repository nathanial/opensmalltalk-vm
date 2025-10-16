/* Extracted from interp.c:43480 (function printInstancesOf). */

/*	Scan the heap printing the oops of any and all objects that are
   instances of aClassOop
 */
/*	useful for VM debugging */

/* SpurMemoryManager>>#printInstancesOf: */

void printInstancesOf(sqInt aClassOop) {
  sqInt classIndex;

  classIndex =
      (long32At((void *)(aClassOop + 4))) & (identityHashHalfWordMask());
  if (classIndex != (isFreeObjectClassIndexPun())) {
    printInstancesWithClassIndex(classIndex);
  }
}