/* Extracted from interp.c:38991 (function mapExtraRoots). */

static void
mapExtraRoots(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    sqInt oop;

	if (shouldRemapObj(GIV(specialObjectsOop))) {
		GIV(validatedIntegerClassFlags) = 0;
		GIV(specialObjectsOop) = remapObj(GIV(specialObjectsOop));
	}
	assert(GIV(remapBufferCount) == 0);

	/* 1 to: remapBufferCount do:
	   [:i | | oop |
	   oop := remapBufferCount at: i.
	   ((self isImmediate: oop) or: [self isFreeObject: oop]) ifFalse:
	   [(self shouldRemapObj: oop) ifTrue:
	   [remapBuffer at: i put: (self remapObj: oop)]]]. */
	for (i = 1; i <= GIV(extraRootCount); i += 1) {
		oop = (GIV(extraRoots)[i])[0];
		if (!((((oop & (tagMask())) != 0))
			 || (((longAt((void *)(oop))) & (classIndexMask())) == (isFreeObjectClassIndexPun())))) {
			if (shouldRemapObj(oop)) {
				(GIV(extraRoots)[i])[0] = (remapObj(oop));
			}
		}
	}
}