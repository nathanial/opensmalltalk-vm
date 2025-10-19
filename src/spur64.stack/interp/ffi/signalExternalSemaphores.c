/* Extracted from interp.c:63371 (function signalExternalSemaphores). */

/*	Signal all requested semaphores. Answer if a context switch has
 * occurred. */

/* StackInterpreter>>#signalExternalSemaphores */

static sqInt signalExternalSemaphores(void) {
  sqInt xArray;

  xArray = fetchPointerofObject(ExternalObjectsArray, specialObjectsOop);
  return doSignalExternalSemaphores((numSlotsOf(xArray)));
}
