# -*- sh -*-

# Note: Fast BitBlt optimizations (SIMD) have been removed from the codebase
bitblt_objs="BitBltPlugin.o"
bitblt_flags=""
arm_arch=""

AC_SUBST(BITBLT_OBJS, $bitblt_objs)
AC_SUBST(BITBLT_FLAGS, $bitblt_flags)
AC_SUBST(ARM_ARCH, $arm_arch)
