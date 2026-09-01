	vfmulcph xmm4, xmm4, xmm2
	vfmulcph xmm4, xmm2
	vfmulcph xmm6, xmm6, xmm6
	vfmulcph ymm2, ymm5, ymm5
	vfmulcph ymm2, ymm5
	vfmulcph ymm6, ymm3, ymm0
	vfmulcph zmm0, zmm1, zmm5
	vfmulcph zmm0, zmm5
	vfmulcph zmm0, zmm7, zword [0x671]

%ifdef ERROR
	vfmulcph xmm10, xmm12, xmm13
	vfmulcph ymm11, ymm10, ymm12
	vfmulcph zmm11, zmm13, zmm8
	vfmulcph xmm30, xmm27, xmm28
	vfmulcph ymm22, ymm18, ymm25
	vfmulcph zmm22, zmm17, zmm26
%endif
