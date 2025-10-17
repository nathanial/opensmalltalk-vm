/* Extracted from interp.c:51056 (function checkOkayInterpreterObjects). */

/*	useful for VM debugging */

	/* StackInterpreter>>#checkOkayInterpreterObjects: */

sqInt
checkOkayInterpreterObjects(sqInt writeBack)
{
    int i;
    sqInt ok;
    sqInt oop;
    sqIntptr_t oopOrZero;

	ok = 1;
	ok = ok && (checkOkayFields(nilObj));
	ok = ok && (checkOkayFields(falseObj));
	ok = ok && (checkOkayFields(trueObj));
	ok = ok && (checkOkayFields(specialObjectsOop));
	ok = ok && (checkOkayFields(messageSelector));
	ok = ok && (checkOkayFields(newMethod));
	ok = ok && (checkOkayFields(lkupClass));
	for (i = 0; i < MethodCacheEntries; i += MethodCacheEntrySize) {
		oopOrZero = methodCache[i + MethodCacheSelector];
		if (oopOrZero) {
			ok = ok && (checkOkayFields(methodCache[i + MethodCacheSelector]));
			ok = ok && (checkOkayFields(methodCache[i + MethodCacheMethod]));
		}
	}
	for (i = 1; i <= remapBufferCount; i += 1) {
		oop = remapBuffer[i];
		if (!(((oop & (tagMask())) != 0))) {
			ok = ok && (checkOkayFields(oop));
		}
	}
	ok = ok && (checkOkayStackZone(writeBack));
	return ok;
}