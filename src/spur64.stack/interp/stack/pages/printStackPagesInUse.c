/* Extracted from interp.c:60954 (function printStackPagesInUse). */

/*	useful for VM debugging */

/* StackInterpreter>>#printStackPagesInUse */

void printStackPagesInUse(void) {
  sqInt i;
  sqInt n;

  n = 0;
  for (i = 0; i < numStackPages; i += 1) {
    if (!(isFree(stackPageAt(i)))) {
      printStackPageuseCount(stackPageAt(i), (n += 1));
      cr();
    }
  }
}