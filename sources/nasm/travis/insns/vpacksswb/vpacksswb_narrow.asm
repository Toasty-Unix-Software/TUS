	vpacksswb xmm0, xmm0, oword [0x552]
	vpacksswb xmm0, oword [0x552]
	vpacksswb xmm4, xmm2, xmm7
	vpacksswb ymm1, ymm0, ymm6
	vpacksswb ymm1, ymm6
	vpacksswb ymm2, ymm2, yword [0x959]
	vpacksswb xmm6, xmm6, xmm4
	vpacksswb xmm6, xmm4
	vpacksswb xmm3, xmm0, oword [0xd6b]
	vpacksswb ymm6, ymm4, ymm4
	vpacksswb ymm6, ymm4
	vpacksswb ymm6, ymm1, yword [0x76b]

%ifdef ERROR
	vpacksswb xmm13, xmm13, xmm13
	vpacksswb ymm12, ymm13, ymm14
	vpacksswb xmm13, xmm8, xmm12
	vpacksswb ymm15, ymm14, ymm15
	vpacksswb xmm31, xmm23, xmm21
	vpacksswb ymm21, ymm21, ymm22
	vpacksswb xmm28, xmm24, xmm21
	vpacksswb ymm26, ymm25, ymm26
%endif
