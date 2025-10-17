/* Extracted from interp.c:38991 (function mapExtraRoots). */

	/* SpurMemoryManager>>#mapExtraRoots */

static void
mapExtraRoots(void)
{
    sqInt i;
    sqInt oop;

	if (shouldRemapObj(specialObjectsOop)) {
		validatedIntegerClassFlags = 0;
		specialObjectsOop = remapObj(specialObjectsOop);
	}
	assert(remapBufferCount == 0);

	/* 1 to: remapBufferCount do:
	   [:i | | oop |
	   oop := remapBufferCount at: i.
	   ((self isImmediate: oop) or: [self isFreeObject: oop]) ifFalse:
	   [(self shouldRemapObj: oop) ifTrue:
	   [remapBuffer at: i put: (self remapObj: oop)]]]. */
	for (i = 1; i <= extraRootCount; i += 1) {
		oop = (extraRoots[i])[0];
		if (!((((oop & (tagMask())) != 0))
			 || (((longAt((void *)(oop))) & (classIndexMask())) == (isFreeObjectClassIndexPun())))) {
			if (shouldRemapObj(oop)) {
				(extraRoots[i])[0] = (remapObj(oop));
			}
		}
	}
}