/* Extracted from interp.c:68640 (function primitiveIncrementalGC). */

/*	Do a quick, incremental garbage collection and return the number of
   bytes immediately available. (Note: more space may be made available by doing
   a full garbage collection.
 */
/*	Do a quick, incremental garbage collection and return the number of
   bytes immediately available. (Note: more space may be made available by doing
   a full garbage collection.
 */

/* StackInterpreterPrimitives>>#primitiveIncrementalGC */

static void primitiveIncrementalGC(void) {
  sqInt integerVal;
  char *sp;

  externalWriteBackHeadFramePointers();

  /* begin scavengingGC */
  scavengingGCTenuringIf(TenureByAge);
  integerVal = ((((totalFreeOldSpace + (scavengeThreshold - freeStart)) -
                  (interpreterAllocationReserveBytes())) < 0)
                    ? 0
                    : ((totalFreeOldSpace + (scavengeThreshold - freeStart)) -
                       (interpreterAllocationReserveBytes())));

  /* begin pop:thenPushInteger: */
  longAtput((sp = stackPointer), (((usqInt)integerVal << 3) | 1));
  stackPointer = sp;
}