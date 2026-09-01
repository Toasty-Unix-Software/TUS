	vpermw xmm1, xmm0, xmm2
	vpermw xmm1, xmm2
	vpermw xmm3, xmm7, xmm0
	vpermw ymm4, ymm6, yword [0x769]
	vpermw ymm4, yword [0x769]
	vpermw ymm5, ymm4, ymm0
	vpermw zmm7, zmm4, zmm1
	vpermw zmm7, zmm1
	vpermw zmm0, zmm4, zmm2

%ifdef ERROR
	vpermw xmm14, xmm10, xmm9
	vpermw ymm13, ymm12, ymm15
	vpermw zmm11, zmm8, zmm12
	vpermw xmm17, xmm24, xmm17
	vpermw ymm24, ymm17, ymm29
	vpermw zmm29, zmm30, zmm24
%endif
