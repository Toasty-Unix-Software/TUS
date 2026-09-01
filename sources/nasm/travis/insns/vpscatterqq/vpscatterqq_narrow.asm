	vpscatterqq [xmm2*1], xmm2
	vpscatterqq [xmm7*1], xmm3
	vpscatterqq [ymm0*1], ymm4
	vpscatterqq [ymm7*1], ymm0
	vpscatterqq [zmm5*1], zmm0
	vpscatterqq [zmm2*1], zmm7

%ifdef ERROR
	vpscatterqq [xmm10*1], xmm12
	vpscatterqq [ymm11*1], ymm11
	vpscatterqq [zmm11*1], zmm14
	vpscatterqq [xmm17*1], xmm28
	vpscatterqq [ymm27*1], ymm17
	vpscatterqq [zmm23*1], zmm23
%endif
