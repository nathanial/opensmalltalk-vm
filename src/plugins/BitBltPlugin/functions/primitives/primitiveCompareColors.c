/* Extracted from BitBltPlugin.c:4147 (function primitiveCompareColors). */
/* BitBltSimulation>>#primitiveCompareColorA:to:test: */
/*	Invoke the copyBits primitive. If the destination is the display, then
        copy it to the screen.
 */

EXPORT(sqInt)
primitiveCompareColors(void) {
  usqIntptr_t colorA;
  usqIntptr_t colorB;
  sqInt rcvr;
  sqInt testID;
  sqInt val;

  val = 0;
  if (!((isPositiveMachineIntegerObject(stackValue(2))) &&
        ((isPositiveMachineIntegerObject(stackValue(1))) &&
         (isIntegerObject((testID = stackValue(0))))))) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  colorA = (BytesPerOop == 4 ? positive32BitValueOf(stackValue(2))
                             : positive64BitValueOf(stackValue(2)));
  colorB = (BytesPerOop == 4 ? positive32BitValueOf(stackValue(1))
                             : positive64BitValueOf(stackValue(1)));
  testID = integerValueOf(testID);
  rcvr = stackValue(3);

  /* Note: primitiveCompareColors not supported (fast path code removed) */
  (void)colorA;
  (void)colorB;
  (void)testID;
  (void)rcvr;
  (void)val;
  return primitiveFailFor(PrimErrUnsupported);
}
