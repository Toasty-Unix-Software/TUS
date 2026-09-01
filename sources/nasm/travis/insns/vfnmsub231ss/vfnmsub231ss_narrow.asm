	vfnmsub231ss xmm7, xmm5, xmm5
	vfnmsub231ss xmm1, xmm0, xmm1
	vfnmsub231ss xmm4, xmm6, xmm2
	vfnmsub231ss xmm5, xmm7, xmm4

%ifdef ERROR
	vfnmsub231ss xmm10, xmm13, xmm9
	vfnmsub231ss xmm10, xmm11, xmm15
	vfnmsub231ss xmm17, xmm24, xmm29
	vfnmsub231ss xmm25, xmm21, xmm30
%endif
