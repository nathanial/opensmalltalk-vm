/* Extracted from interp.c:61173 (function print). */

	/* StackInterpreter>>#print: */

void
print(char *s)
{
	fprintf(transcript,
			"%s",
			s);
}