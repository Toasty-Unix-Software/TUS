	vpblendmb xmm4, xmm0, oword [0x7ff]
	vpblendmb xmm5, xmm7, xmm6
	vpblendmb ymm1, ymm1, yword [0xcca]
	vpblendmb ymm1, ymm6, ymm7
	vpblendmb zmm6, zmm4, zmm7
	vpblendmb zmm5, zmm3, zmm0

%ifdef ERROR
	vpblendmb xmm11, xmm11, xmm10
	vpblendmb ymm8, ymm14, ymm13
	vpblendmb zmm9, zmm14, zmm9
	vpblendmb xmm27, xmm17, xmm29
	vpblendmb ymm25, ymm29, ymm18
	vpblendmb zmm16, zmm21, zmm24
%endif
