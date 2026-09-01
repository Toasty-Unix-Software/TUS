	vrsqrtph xmm1, xmm0
	vrsqrtph xmm3, xmm5
	vrsqrtph ymm1, ymm4
	vrsqrtph ymm5, yword [0x934]
	vrsqrtph zmm4, zmm5
	vrsqrtph zmm1, zmm3

%ifdef ERROR
	vrsqrtph xmm13, xmm12
	vrsqrtph ymm13, ymm9
	vrsqrtph zmm9, zmm8
	vrsqrtph xmm31, xmm24
	vrsqrtph ymm31, ymm21
	vrsqrtph zmm16, zmm31
%endif
