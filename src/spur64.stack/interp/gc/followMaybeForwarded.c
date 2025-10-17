/* Extracted from interp.c:35680 (function followMaybeForwarded). */

/* SpurMemoryManager>>#followMaybeForwarded: */

static sqInt followMaybeForwarded(sqInt objOop) {
  return (/* isOopForwarded: */
          isOopForwarded(objOop))
             ? followForwarded(objOop)
             : objOop;
}
