/* Extracted from interp.c:55096 (function isLiveContext). */

/*	Answer if the argument, which can be any object, is a live context. */

/* StackInterpreter>>#isLiveContext: */

static sqInt isLiveContext(sqInt oop) {
  assert(!((isOopForwarded(oop))));
  if (!(/* isContext: */
        ((!(oop & (tagMask())))) &&
        (((longAt((void *)(oop))) & (classIndexMask())) ==
         ClassMethodContextCompactIndex))) {
    return 0;
  }
  if ((!((fetchPointerofObject(SenderIndex, oop)) &
         (tagMask())))) {
    return ((((fetchPointerofObject(InstructionPointerIndex, oop))) &
             7) == 1);
  }
  return !(isWidowedContext(oop));
}