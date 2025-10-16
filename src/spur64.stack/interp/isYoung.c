/* Extracted from interp.c:37925 (function isYoung). */

sqInt
isYoung(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return ((!(oop & (tagMask()))))
	 && (oopisLessThan(oop, GIV(oldSpaceStart)));
}