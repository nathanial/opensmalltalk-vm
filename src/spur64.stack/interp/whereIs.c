/* Extracted from interp.c:65829 (function whereIs). */

char *
whereIs(sqInt anOop)
{
    char *where;

	if ((where = whereIsMaybeHeapThing(anOop))) {
		return where;
	}
	if ((where = whereIsMaybeStackThing(anOop))) {
		return where;
	}
	return " is no where obvious";
}