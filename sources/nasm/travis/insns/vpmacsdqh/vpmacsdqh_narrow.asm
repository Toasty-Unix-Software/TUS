	vpmacsdqh xmm3, xmm4, xmm4, xmm6
	vpmacsdqh xmm3, xmm4, xmm6
	vpmacsdqh xmm0, xmm4, oword [0x216], xmm0

%ifdef ERROR
	vpmacsdqh xmm13, xmm15, xmm11, xmm12
%endif
