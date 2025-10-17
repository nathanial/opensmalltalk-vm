/* Extracted from interp.c:49882 (function activeProcess). */

/*	Answer the current activeProcess. */
/*	useful for VM debugging */

/* StackInterpreter>>#activeProcess */

sqInt activeProcess(void) {
  sqInt objOop;

  objOop =
      fetchPointerofObject(ValueIndex, fetchPointerofObject(SchedulerAssociation, specialObjectsOop));

  return fetchPointerofObject(ActiveProcessIndex, objOop);
}