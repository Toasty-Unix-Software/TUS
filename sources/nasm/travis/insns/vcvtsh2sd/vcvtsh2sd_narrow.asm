	vcvtsh2sd xmm3, xmm7, xmm6
	vcvtsh2sd xmm3, xmm6
	vcvtsh2sd xmm2, xmm2, xmm2

%ifdef ERROR
	vcvtsh2sd xmm10, xmm8, xmm15
	vcvtsh2sd xmm28, xmm23, xmm18
%endif
