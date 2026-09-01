	vfmsubadd231ph xmm6, xmm0, xmm3
	vfmsubadd231ph xmm6, xmm3
	vfmsubadd231ph xmm7, xmm6, oword [0x232]
	vfmsubadd231ph ymm2, ymm6, yword [0x282]
	vfmsubadd231ph ymm2, yword [0x282]
	vfmsubadd231ph ymm1, ymm7, ymm0
	vfmsubadd231ph zmm0, zmm3, zmm1
	vfmsubadd231ph zmm0, zmm1
	vfmsubadd231ph zmm5, zmm0, zword [0x7ce]

%ifdef ERROR
	vfmsubadd231ph xmm15, xmm10, xmm9
	vfmsubadd231ph ymm10, ymm14, ymm10
	vfmsubadd231ph zmm8, zmm12, zmm9
	vfmsubadd231ph xmm25, xmm16, xmm25
	vfmsubadd231ph ymm19, ymm16, ymm27
	vfmsubadd231ph zmm27, zmm28, zmm24
%endif
