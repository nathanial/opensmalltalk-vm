/* Extracted from B2DPlugin.c:3787 (function isFillOkay). */

static sqInt isFillOkay(sqInt fill) {
  return (fill == 0) ||
         (((fill & 0xFF000000U) != 0) ||
          ((/* isObject: */
            (fill >= 0) && (fill < objUsed)) &&
           (/* isFill: */
            ((fill & 0xFF000000U) != 0) ||
            ((((objBuffer[fill + GEObjectType]) & GEPrimitiveTypeMask) &
              GEPrimitiveFillMask) != 0))));
}