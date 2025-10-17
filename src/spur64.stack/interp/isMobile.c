/* Extracted from interp.c:47704 (function isMobile). */

/* SpurPlanningCompactor>>#isMobile: */

static sqInt isMobile(sqInt obj) {
  return (oopisGreaterThanOrEqualToandLessThanOrEqualTo(obj, mobileStart,
                                                        lastMobileObject)) &&
         (!(((byteAt((void *)(obj + (formatFieldByteOffset())))) &
             (1U << (pinnedBitByteShift()))) != 0));
}