/* Extracted from interp.c:27390 (function isSmallFloatValue). */

/* Spur64BitMemoryManager>>#isSmallFloatValue: */

static NoDbgRegParms int isSmallFloatValue(double aFloat) {
  usqInt exponent;
  usqLong rawFloat;

  memcpy((&rawFloat), (&aFloat), sizeof(rawFloat));
  exponent = ((rawFloat) >> (smallFloatMantissaBits())) & 0x7FF;
  return (exponent > (smallFloatExponentOffset())
              ? exponent <= (0xFF + (smallFloatExponentOffset()))
              : (rawFloat & ((1ULL << (smallFloatMantissaBits())) - 1)
                     ? exponent == (smallFloatExponentOffset())
                     : exponent == 0));
}