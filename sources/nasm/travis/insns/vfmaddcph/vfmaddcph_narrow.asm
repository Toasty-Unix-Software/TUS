	vfmaddcph xmm2, xmm6, oword [0x451]
	vfmaddcph xmm2, oword [0x451]
	vfmaddcph xmm5, xmm6, xmm4
	vfmaddcph ymm1, ymm0, ymm1
	vfmaddcph ymm1, ymm1
	vfmaddcph ymm5, ymm5, ymm2
	vfmaddcph zmm6, zmm3, zmm7
	vfmaddcph zmm6, zmm7
	vfmaddcph zmm0, zmm6, zmm5

%ifdef ERROR
	vfmaddcph xmm8, xmm12, xmm14
	vfmaddcph ymm9, ymm14, ymm8
	vfmaddcph zmm8, zmm10, zmm10
	vfmaddcph xmm25, xmm21, xmm28
	vfmaddcph ymm26, ymm31, ymm26
	vfmaddcph zmm19, zmm18, zmm31
%endif
