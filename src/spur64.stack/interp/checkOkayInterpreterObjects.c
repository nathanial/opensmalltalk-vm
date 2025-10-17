/* Extracted from interp.c:51056 (function checkOkayInterpreterObjects). */

/*	useful for VM debugging */

	/* StackInterpreter>>#checkOkayInterpreterObjects: */

sqInt
checkOkayInterpreterObjects(sqInt writeBack)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    int i;
    sqInt ok;
    sqInt oop;
    sqIntptr_t oopOrZero;

	ok = 1;
	ok = ok && (checkOkayFields(GIV(nilObj)));
	ok = ok && (checkOkayFields(GIV(falseObj)));
	ok = ok && (checkOkayFields(GIV(trueObj)));
	ok = ok && (checkOkayFields(GIV(specialObjectsOop)));
	ok = ok && (checkOkayFields(GIV(messageSelector)));
	ok = ok && (checkOkayFields(GIV(newMethod)));
	ok = ok && (checkOkayFields(GIV(lkupClass)));
	for (i = 0; i < MethodCacheEntries; i += MethodCacheEntrySize) {
		oopOrZero = GIV(methodCache)[i + MethodCacheSelector];
		if (oopOrZero) {
			ok = ok && (checkOkayFields(GIV(methodCache)[i + MethodCacheSelector]));
			ok = ok && (checkOkayFields(GIV(methodCache)[i + MethodCacheMethod]));
		}
	}
	for (i = 1; i <= GIV(remapBufferCount); i += 1) {
		oop = GIV(remapBuffer)[i];
		if (!(((oop & (tagMask())) != 0))) {
			ok = ok && (checkOkayFields(oop));
		}
	}
	ok = ok && (checkOkayStackZone(writeBack));
	return ok;
}