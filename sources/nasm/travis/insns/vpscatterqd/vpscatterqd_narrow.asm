	vpscatterqd [xmm5*1], xmm5
	vpscatterqd [xmm5*1], xmm0
	vpscatterqd [ymm3*1], xmm5
	vpscatterqd [ymm0*1], xmm2
	vpscatterqd [zmm7*1], ymm4
	vpscatterqd [zmm0*1], ymm4

%ifdef ERROR
	vpscatterqd [xmm14*1], xmm9
	vpscatterqd [ymm9*1], xmm10
	vpscatterqd [zmm15*1], ymm9
	vpscatterqd [xmm21*1], xmm19
	vpscatterqd [ymm28*1], xmm22
	vpscatterqd [zmm20*1], ymm25
%endif
