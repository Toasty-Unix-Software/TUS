	vscatterqpd [xmm7*1], xmm7
	vscatterqpd [xmm7*1], xmm2
	vscatterqpd [ymm5*1], ymm5
	vscatterqpd [ymm5*1], ymm6
	vscatterqpd [zmm3*1], zmm4
	vscatterqpd [zmm6*1], zmm4

%ifdef ERROR
	vscatterqpd [xmm11*1], xmm10
	vscatterqpd [ymm12*1], ymm13
	vscatterqpd [zmm12*1], zmm10
	vscatterqpd [xmm29*1], xmm22
	vscatterqpd [ymm30*1], ymm24
	vscatterqpd [zmm31*1], zmm25
%endif
