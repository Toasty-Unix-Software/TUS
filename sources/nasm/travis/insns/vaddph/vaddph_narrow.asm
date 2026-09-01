	vaddph xmm0, xmm1, oword [0x2cc]
	vaddph xmm0, oword [0x2cc]
	vaddph xmm6, xmm2, xmm5
	vaddph ymm7, ymm0, yword [0x2ea]
	vaddph ymm7, yword [0x2ea]
	vaddph ymm4, ymm0, ymm2
	vaddph zmm5, zmm6, zmm4
	vaddph zmm5, zmm4
	vaddph zmm4, zmm7, zmm1

%ifdef ERROR
	vaddph xmm14, xmm13, xmm9
	vaddph ymm9, ymm9, ymm14
	vaddph zmm9, zmm10, zmm9
	vaddph xmm20, xmm22, xmm26
	vaddph ymm26, ymm20, ymm16
	vaddph zmm20, zmm28, zmm22
%endif
