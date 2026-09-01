	vpminuq xmm2, xmm2, oword [0x19a]
	vpminuq xmm2, oword [0x19a]
	vpminuq xmm6, xmm5, xmm7
	vpminuq ymm0, ymm5, ymm3
	vpminuq ymm0, ymm3
	vpminuq ymm1, ymm0, ymm1
	vpminuq zmm5, zmm1, zmm5
	vpminuq zmm5, zmm5
	vpminuq zmm7, zmm5, zmm1

%ifdef ERROR
	vpminuq xmm10, xmm13, xmm8
	vpminuq ymm9, ymm10, ymm11
	vpminuq zmm9, zmm14, zmm8
	vpminuq xmm21, xmm28, xmm29
	vpminuq ymm22, ymm31, ymm23
	vpminuq zmm17, zmm29, zmm25
%endif
