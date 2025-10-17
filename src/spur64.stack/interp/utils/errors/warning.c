/* Extracted from interp.c:39 (function warning). */

EXPORT(void)
warning(
    const char *s) { /* Print an error message but don't necessarily exit. */
  if (erroronwarn)
    error(s);
  if (warnpid)
    printf("\n%s pid %ld\n", s, (long)warnpid);
  else
    printf("\n%s\n", s);
}