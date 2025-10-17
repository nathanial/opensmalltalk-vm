/* Extracted from interp.c:55602 (function loadInitialContext). */

/* StackInterpreter>>#loadInitialContext */

void loadInitialContext(void) {
  sqInt activeContext;
  sqInt activeProc;
  sqInt obj;
  sqInt objOop;

  runLeakCheckerFor(GCModeFull);

  /* primitiveSuspend needs to know the class of LinkedList */

  /* begin getClassTagOfLinkedList */
  objOop = fetchPointerofObject(
      ValueIndex,
      fetchPointerofObject(SchedulerAssociation, specialObjectsOop));
  objOop = fetchPointerofObject(ProcessListsIndex, objOop);
  obj = fetchPointerofObject(0U, objOop);
  classLinkedListClassTag = (longAt((void *)(obj))) & (classIndexMask());
  objOop = fetchPointerofObject(
      ValueIndex,
      fetchPointerofObject(SchedulerAssociation, specialObjectsOop));
  activeProc = fetchPointerofObject(ActiveProcessIndex, objOop);
  activeContext = fetchPointerofObject(SuspendedContextIndex, activeProc);
  marryContextInNewStackPageAndInitializeInterpreterRegisters(activeContext);
}