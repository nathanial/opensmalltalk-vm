/* Extracted from interp.c:44872 (function pushRemappableOop). */

void
pushRemappableOop(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(addressCouldBeOop(oop));
	GIV(remapBuffer)[(GIV(remapBufferCount) += 1)] = oop;
	if (!(GIV(remapBufferCount) <= RemapBufferSize)) {
		error("remapBuffer overflow");
	}
}