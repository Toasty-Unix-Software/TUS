	vpunpckhwd xmm3, xmm0, xmm6
	vpunpckhwd xmm3, xmm6
	vpunpckhwd xmm0, xmm2, oword [0x417]
	vpunpckhwd ymm3, ymm6, ymm5
	vpunpckhwd ymm3, ymm5
	vpunpckhwd ymm1, ymm6, ymm3
	vpunpckhwd xmm3, xmm5, oword [0xc7d]
	vpunpckhwd xmm3, oword [0xc7d]
	vpunpckhwd xmm3, xmm5, xmm0
	vpunpckhwd ymm3, ymm5, ymm2
	vpunpckhwd ymm3, ymm2
	vpunpckhwd ymm1, ymm5, ymm1

%ifdef ERROR
	vpunpckhwd xmm12, xmm9, xmm13
	vpunpckhwd ymm14, ymm9, ymm11
	vpunpckhwd xmm8, xmm8, xmm12
	vpunpckhwd ymm8, ymm15, ymm8
	vpunpckhwd xmm23, xmm28, xmm27
	vpunpckhwd ymm31, ymm29, ymm28
	vpunpckhwd xmm20, xmm21, xmm24
	vpunpckhwd ymm30, ymm28, ymm20
%endif
