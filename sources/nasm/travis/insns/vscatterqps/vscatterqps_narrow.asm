	vscatterqps [xmm7*1], xmm4
	vscatterqps [xmm2*1], xmm7
	vscatterqps [ymm3*1], xmm5
	vscatterqps [ymm4*1], xmm3
	vscatterqps [zmm4*1], ymm0
	vscatterqps [zmm2*1], ymm7

%ifdef ERROR
	vscatterqps [xmm10*1], xmm11
	vscatterqps [ymm9*1], xmm13
	vscatterqps [zmm12*1], ymm14
	vscatterqps [xmm31*1], xmm17
	vscatterqps [ymm22*1], xmm28
	vscatterqps [zmm26*1], ymm25
%endif
