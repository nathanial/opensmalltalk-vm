/* Extracted from BitBltPlugin.c:1808 (function copyBitsFallback). */
/* BitBltSimulation>>#copyBits:Fallback: */
/*	This version of the inner loop assumes noSource = false. */
/*  Note: Fast path optimizations removed - function is now a no-op */

void copyBitsFallback(operation_t *op, unsigned int flags) {
  /* Note: This function is no longer used since fast path optimizations were removed */
  (void)op;
  (void)flags;
  return;
}
