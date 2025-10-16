/* Extracted from interp.c:52072 (function doSignalSemaphoreWithIndex). */

/*	Signal the external semaphore with the given index. Answer if a context
        switch occurs as a result. Do not bounds check. This has been done in
   the caller.  */

/* StackInterpreter>>#doSignalSemaphoreWithIndex: */

sqInt doSignalSemaphoreWithIndex(sqInt index) {
  sqInt sema;
  sqInt xArray;

  xArray = fetchPointerofObject(ExternalObjectsArray, specialObjectsOop);
  assert(isArray(xArray));

  /* Note: semaphore indices are 1-based */
  sema = fetchPointerofObject(index - 1, xArray);
  assert(!(isOopForwarded(sema)));
  return (/* isSemaphoreOop: */
          ((!(sema & (tagMask())))) &&
          (((longAt((void *)(sema))) & (classIndexMask())) ==
           (rawHashBitsOf(
               fetchPointerofObject(ClassSemaphore, specialObjectsOop))))) &&
         (synchronousSignal(sema));
}