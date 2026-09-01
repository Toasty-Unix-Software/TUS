	vfcmulcph xmm6, xmm3, xmm3
	vfcmulcph xmm6, xmm3
	vfcmulcph xmm6, xmm7, xmm6
	vfcmulcph ymm3, ymm5, ymm3
	vfcmulcph ymm3, ymm3
	vfcmulcph ymm7, ymm7, yword [0xb9f]
	vfcmulcph zmm1, zmm2, zmm0
	vfcmulcph zmm1, zmm0
	vfcmulcph zmm1, zmm1, zword [0x76c]

%ifdef ERROR
	vfcmulcph xmm13, xmm8, xmm8
	vfcmulcph ymm15, ymm8, ymm14
	vfcmulcph zmm14, zmm13, zmm12
	vfcmulcph xmm21, xmm26, xmm18
	vfcmulcph ymm26, ymm16, ymm25
	vfcmulcph zmm31, zmm23, zmm18
%endif
