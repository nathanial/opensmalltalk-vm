/* Extracted from interp.c:45592 (function shouldRemapOop). */

/*	Answer if the oop should be scavenged.. The method is called
        shouldRemapOop: for compatibility with ObjectMemory. */

/* SpurMemoryManager>>#shouldRemapOop: */

sqInt shouldRemapOop(sqInt oop) {
  return ((!(oop & (tagMask())))) && (shouldRemapObj(oop));
}