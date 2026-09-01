default rel
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
	vpacksswb xmm13, xmm13, xmm13
	vpacksswb ymm12, ymm13, ymm14
	vpacksswb xmm13, xmm8, xmm12
	vpacksswb ymm15, ymm14, ymm15
	vpacksswb xmm31, xmm23, xmm21
	vpacksswb ymm21, ymm21, ymm22
	vpacksswb xmm28, xmm24, xmm21
	vpacksswb ymm26, ymm25, ymm26
	vpacksswb xmm6{k7}, xmm5, xmm2
	vpacksswb ymm3{k1}, ymm6, yword [0x7df]
	vpacksswb zmm5{k2}, zmm0, zmm6
	vpacksswb xmm6{k1}{z}, xmm6, xmm2
	vpacksswb ymm1{k6}{z}, ymm0, yword [0x6d0]
	vpacksswb zmm3{k7}{z}, zmm4, zword [0x917]
	vpacksswb xmm2, xmm6, oword [eax+1]
	vpacksswb xmm1, xmm3, oword [eax+64]
	vpacksswb ymm5, ymm1, yword [eax+1]
	vpacksswb ymm1, ymm0, yword [eax+64]
	vpacksswb xmm1, xmm3, oword [eax+1]
	vpacksswb xmm0, xmm3, oword [eax+64]
	vpacksswb ymm3, ymm2, yword [eax+1]
	vpacksswb ymm1, ymm0, yword [eax+64]
	vpacksswb zmm1, zmm1, zword [eax+1]
	vpacksswb zmm6, zmm5, zword [eax+64]
	vpacksswb xmm5, xmm5, [0x405]
	vpacksswb ymm4, ymm0, [0xebb]
	vpacksswb xmm2, xmm5, [0x114]
	vpacksswb ymm5, ymm7, [0x938]
	vpacksswb zmm3, zmm5, [0x7f1]
