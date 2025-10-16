/* Extracted from interp.c:54 (function warninginat). */

EXPORT(void)
warninginat(const char *s, const char *f,
            int l) { /* ditto with function name & line number. */
  /* use alloca to call warning so one does not have to remember to set multiple
   * breakpoints... */
  char *sl = alloca(strlen(s) + strlen(f) + 17);
  sprintf(sl, "%s %s %d", s, f, l);
  warning(sl);
}