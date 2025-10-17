/* Extracted from interp.c:60178 (function printMethodCacheFor). */

/*	useful for VM debugging */

/* StackInterpreter>>#printMethodCacheFor: */

void printMethodCacheFor(sqInt thing) {
  sqIntptr_t c;
  int i;
  sqIntptr_t m;
  sqInt n;
  sqIntptr_t p;
  sqIntptr_t s;

  n = 0;
  for (i = 0; i < MethodCacheSize; i += MethodCacheEntrySize) {
    s = methodCache[i + MethodCacheSelector];
    c = methodCache[i + MethodCacheClass];
    m = methodCache[i + MethodCacheMethod];
    p = methodCache[i + MethodCachePrimFunction];
    if (((thing == -1) ||
         ((s == thing) || ((c == thing) || ((p == thing) || (m == thing))))) &&
        ((/* addressCouldBeOop: */
          (((s & (tagMask())) != 0)) || (addressCouldBeObj(s))) &&
         ((c != 0) && ((addressCouldBeClassObj(c)) ||
                       (addressCouldBeClassObj(classForClassTag(c))))))) {
      n += 1;
      fprintf(transcript, "%d %x\n\t", ((int)i), ((int)i));
      if (((byteAt((void *)(s + (formatFieldByteOffset())))) &
           (formatMask())) >= (firstByteFormat())) {
        fprintf(transcript, "%p %.*s\n", ((void *)s),
                ((int)(numBytesOfBytes(s))),
                ((char *)(firstIndexableField(s))));
      } else {
        shortPrintOop(s);
      }

      /* begin tab */
      printChar('\t');
      if (addressCouldBeClassObj(c)) {
        shortPrintOop(c);
      } else {
        printNum(c);

        /* begin space */
        printChar(' ');

        /* printHexnp: */
        fprintf(transcript, "%p", ((void *)c));
        printChar(' ');
        shortPrintOop(classForClassTag(c));
      }

      /* begin tab */
      printChar('\t');
      shortPrintOop(m);

      /* begin tab */
      printChar('\t');
      if (p > 0x400) {
        /* printHexnp: */
        fprintf(transcript, "%p", ((void *)p));
      } else {
        printNum(p);
      }
      cr();
    }
  }
  if (n > 1) {
    printNum(n);
    cr();
  }
}