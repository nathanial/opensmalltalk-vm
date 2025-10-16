/* Extracted from interp.c:50178 (function assertValidExecutionPointers). */

/* StackInterpreter>>#assertValidExecutionPointe:r:s: */

void assertValidExecutionPointers(usqInt lip, char *lifp, char *lisp) {
  assertValidExecutionPointersimbarline(
      lip, lifp, lisp, 1 /* (isMachineCodeFrame: not) */, __LINE__);
}