/* Extracted from interp.c:55504 (function isMemberOf). */

sqInt
isMemberOf(sqInt oop, char *className)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oopClass;
    sqInt tagBits;

	oopClass = /* fetchClassOf: */
			((tagBits = oop & (tagMask()))
				? longAt((void *)((GIV(classTableFirstPage) + BaseHeaderSize) + ((((usqInt)(tagBits) << (shiftForWord()))))))
				: fetchClassOfNonImm(oop));
	return classNameOfIs(oopClass, className);
}