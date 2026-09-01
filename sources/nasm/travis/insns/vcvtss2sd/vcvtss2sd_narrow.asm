	vcvtss2sd xmm3, xmm6, xmm1
	vcvtss2sd xmm3, xmm1
	vcvtss2sd xmm5, xmm1, xmm1
	vcvtss2sd xmm7, xmm1, xmm3
	vcvtss2sd xmm7, xmm3
	vcvtss2sd xmm6, xmm0, xmm6

%ifdef ERROR
	vcvtss2sd xmm13, xmm14, xmm8
	vcvtss2sd xmm15, xmm12, xmm13
	vcvtss2sd xmm22, xmm25, xmm31
	vcvtss2sd xmm16, xmm18, xmm23
%endif
