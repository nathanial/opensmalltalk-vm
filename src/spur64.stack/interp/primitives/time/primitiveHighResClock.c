/* Extracted from interp.c:17714 (function primitiveHighResClock). */

/*	Return the value of the high resolution clock if this system has any.
        The exact frequency of the high res clock is undefined specifically so
        that we can use
        processor dependent instructions (like RDTSC). The only use for the high
        res clock is for
        profiling where we can allocate time based on sub-msec resolution of the
        high res clock.
        If no high-resolution counter is available, the platform should return
        zero. ar 6/22/2007 */

/* InterpreterPrimitives>>#primitiveHighResClock */

EXPORT(sqInt)
primitiveHighResClock(void) {
  sqInt oop;
  char *sp;

  oop = positive64BitIntegerFor(ioHighResClock());

  /* begin methodReturnValue: */
  assert(!((failed())));
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            oop);
  stackPointer = sp;
  return 0;
}