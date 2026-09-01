	vsubph xmm6, xmm2, xmm2
	vsubph xmm6, xmm2
	vsubph xmm4, xmm5, oword [0xa80]
	vsubph ymm1, ymm1, yword [0xea5]
	vsubph ymm1, yword [0xea5]
	vsubph ymm2, ymm7, yword [0x482]
	vsubph zmm3, zmm6, zmm4
	vsubph zmm3, zmm4
	vsubph zmm4, zmm5, zmm5

%ifdef ERROR
	vsubph xmm9, xmm8, xmm8
	vsubph ymm13, ymm12, ymm13
	vsubph zmm9, zmm10, zmm14
	vsubph xmm28, xmm31, xmm17
	vsubph ymm25, ymm26, ymm16
	vsubph zmm16, zmm26, zmm22
%endif
