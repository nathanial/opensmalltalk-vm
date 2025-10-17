/* Extracted from interp.c:59416 (function printCallStack). */

/*	useful for VM debugging */

/* StackInterpreter>>#printCallStack */

void printCallStack(void) {
  char *fp;

  if ((fp = framePointer)) {
    printCallStackFP(fp);
  } else {
    printCallStackOf(
        fetchPointerofObject(SuspendedContextIndex, activeProcess()));
  }
}