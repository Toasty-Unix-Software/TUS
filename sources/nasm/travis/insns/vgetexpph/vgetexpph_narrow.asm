	vgetexpph xmm4, xmm4
	vgetexpph xmm5, xmm3
	vgetexpph ymm3, yword [0x5ac]
	vgetexpph ymm0, ymm4
	vgetexpph zmm7, zword [0x69d]
	vgetexpph zmm6, zword [0x397]

%ifdef ERROR
	vgetexpph xmm13, xmm12
	vgetexpph ymm8, ymm14
	vgetexpph zmm8, zmm8
	vgetexpph xmm26, xmm17
	vgetexpph ymm22, ymm21
	vgetexpph zmm22, zmm22
%endif
