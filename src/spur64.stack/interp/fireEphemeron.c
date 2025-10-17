/* Extracted from interp.c:35320 (function fireEphemeron). */

/* SpurMemoryManager>>#fireEphemeron: */

static void fireEphemeron(sqInt ephemeron) {
  queueMourner(ephemeron);

  /* begin setFormatOf:to: */
  assert((((nonIndexablePointerFormat()) >= 0) &&
          ((nonIndexablePointerFormat()) <= (formatMask()))));
  byteAtput((void *)(ephemeron + (formatFieldByteOffset())),
            ((byteAt((void *)(ephemeron + (formatFieldByteOffset())))) &
             (0xFF - (formatMask()))) +
                (nonIndexablePointerFormat()));

  /* begin signalFinalization: */
  forceInterruptCheck();
  pendingFinalizationSignals += 1;
}