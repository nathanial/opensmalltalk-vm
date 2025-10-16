/* Extracted from interp.c:47 (function warningat). */

EXPORT(void)
warningat(const char *s, int l) { /* ditto with line number. */
	/* use alloca to call warning so one does not have to remember to set multiple breakpoints... */
	char *sl = alloca(strlen(s) + 16);
	sprintf(sl, "%s %d", s, l);
	warning(sl);
}