	vpandd xmm3, xmm5, xmm4
	vpandd xmm3, xmm4
	vpandd xmm2, xmm0, xmm5
	vpandd ymm0, ymm4, yword [0xc9c]
	vpandd ymm0, yword [0xc9c]
	vpandd ymm2, ymm2, ymm1
	vpandd zmm5, zmm1, zmm3
	vpandd zmm5, zmm3
	vpandd zmm6, zmm2, zmm1

%ifdef ERROR
	vpandd xmm8, xmm12, xmm13
	vpandd ymm15, ymm15, ymm10
	vpandd zmm9, zmm14, zmm14
	vpandd xmm24, xmm18, xmm29
	vpandd ymm19, ymm30, ymm22
	vpandd zmm24, zmm20, zmm19
%endif
