	vcvtsh2ss xmm2, xmm3, xmm5
	vcvtsh2ss xmm2, xmm5
	vcvtsh2ss xmm1, xmm6, xmm2

%ifdef ERROR
	vcvtsh2ss xmm12, xmm13, xmm8
	vcvtsh2ss xmm28, xmm22, xmm25
%endif
