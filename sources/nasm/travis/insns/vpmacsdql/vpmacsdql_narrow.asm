	vpmacsdql xmm0, xmm3, xmm4, xmm6
	vpmacsdql xmm0, xmm4, xmm6
	vpmacsdql xmm3, xmm2, oword [0xc34], xmm2

%ifdef ERROR
	vpmacsdql xmm13, xmm11, xmm11, xmm12
%endif
