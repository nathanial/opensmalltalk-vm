/* Extracted from interp.c:61173 (function print). */

void
print(char *s)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	fprintf(GIV(transcript),
			"%s",
			s);
}