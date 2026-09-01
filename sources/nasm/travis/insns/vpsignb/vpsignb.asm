default rel
	vpsignb xmm0, xmm2, oword [0xb8a]
	vpsignb xmm0, oword [0xb8a]
	vpsignb xmm7, xmm4, xmm0
	vpsignb ymm2, ymm7, ymm7
	vpsignb ymm2, ymm7
	vpsignb ymm1, ymm6, ymm4
	vpsignb xmm9, xmm11, xmm8
	vpsignb ymm11, ymm9, ymm12
	vpsignb xmm3, xmm1, oword [eax+1]
	vpsignb xmm5, xmm3, oword [eax+64]
	vpsignb ymm6, ymm0, yword [eax+1]
	vpsignb ymm0, ymm6, yword [eax+64]
	vpsignb xmm7, xmm0, [0x49b]
	vpsignb ymm7, ymm0, [0x630]
