	vfcmaddcph xmm3, xmm3, xmm0
	vfcmaddcph xmm3, xmm0
	vfcmaddcph xmm3, xmm1, oword [0x588]
	vfcmaddcph ymm5, ymm2, ymm4
	vfcmaddcph ymm5, ymm4
	vfcmaddcph ymm0, ymm4, ymm3
	vfcmaddcph zmm6, zmm2, zmm1
	vfcmaddcph zmm6, zmm1
	vfcmaddcph zmm5, zmm7, zmm0

%ifdef ERROR
	vfcmaddcph xmm8, xmm9, xmm13
	vfcmaddcph ymm11, ymm8, ymm9
	vfcmaddcph zmm8, zmm8, zmm13
	vfcmaddcph xmm23, xmm30, xmm26
	vfcmaddcph ymm20, ymm25, ymm26
	vfcmaddcph zmm30, zmm26, zmm27
%endif
