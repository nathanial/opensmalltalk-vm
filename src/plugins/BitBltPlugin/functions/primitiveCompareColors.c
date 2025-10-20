/* Extracted from BitBltPlugin.c:4147 (function primitiveCompareColors). */

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
#if ENABLE_FAST_BLT
  if (!(loadBitBltFromwarping(rcvr, 0))) {
    return primitiveFailFor(PrimErrBadReceiver);
  }
  clipRange();
  if ((bbW <= 0) || (bbH <= 0)) {
    return primitiveFail();
  }

  /* zero width or height; noop */

  compare_operation_t op;
  op.matchRule = testID & 3;
  op.tally = testID & (1u << 3);
  op.srcA.bits = (void *)sourceBits;
  op.srcA.pitch = sourcePitch;
  op.srcA.depth = sourceDepth;
  op.srcA.msb = sourceMSB;
  op.srcA.x = sx;
  op.srcA.y = sy;
  op.srcB.bits = (void *)destBits;
  op.srcB.pitch = destPitch;
  op.srcB.depth = destDepth;
  op.srcB.msb = destMSB;
  op.srcB.x = dx;
  op.srcB.y = dy;
  op.width = bbW;
  op.height = bbH;
  op.colorA = colorA;
  op.colorB = colorB;

  val = compareColorsDispatch(&op);
  if (!(failed())) {
    methodReturnValue(positive32BitIntegerFor(val));
  }
  return null;
#else // ENABLE_FAST_BLT
  primitiveFail();
#endif

  if (!(failed())) {
    pop(3);
  }
  return null;
}