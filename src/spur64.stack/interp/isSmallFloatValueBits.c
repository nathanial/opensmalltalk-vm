/* Extracted from interp.c:27376 (function isSmallFloatValueBits). */

/* Spur64BitMemoryManager>>#isSmallFloatValueBits: */

static NoDbgRegParms int isSmallFloatValueBits(usqLong rawFloatBits) {
  usqInt exponent;

  exponent = ((rawFloatBits) >> (smallFloatMantissaBits())) & 0x7FF;
  return (exponent > (smallFloatExponentOffset())
              ? exponent <= (0xFF + (smallFloatExponentOffset()))
              : (rawFloatBits & ((1ULL << (smallFloatMantissaBits())) - 1)
                     ? exponent == (smallFloatExponentOffset())
                     : exponent == 0));
}