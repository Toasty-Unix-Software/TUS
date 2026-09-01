	vpexpandw xmm2, xmm3
	vpexpandw xmm1, oword [0x632]
	vpexpandw ymm3, ymm2
	vpexpandw ymm5, ymm0
	vpexpandw zmm6, zword [0xf22]
	vpexpandw zmm3, zmm1

%ifdef ERROR
	vpexpandw xmm9, xmm13
	vpexpandw ymm15, ymm8
	vpexpandw zmm14, zmm15
	vpexpandw xmm25, xmm29
	vpexpandw ymm24, ymm25
	vpexpandw zmm28, zmm27
%endif
