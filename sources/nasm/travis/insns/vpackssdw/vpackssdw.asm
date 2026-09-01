default rel
	vpackssdw xmm5, xmm2, oword [0x39b]
	vpackssdw xmm5, oword [0x39b]
	vpackssdw xmm6, xmm5, xmm7
	vpackssdw ymm2, ymm3, ymm2
	vpackssdw ymm2, ymm2
	vpackssdw ymm5, ymm6, ymm5
	vpackssdw xmm6, xmm0, xmm2
	vpackssdw xmm6, xmm2
	vpackssdw xmm2, xmm1, xmm4
	vpackssdw ymm5, ymm6, ymm0
	vpackssdw ymm5, ymm0
	vpackssdw ymm3, ymm0, ymm3
	vpackssdw xmm14, xmm13, xmm9
	vpackssdw ymm12, ymm10, ymm8
	vpackssdw xmm12, xmm11, xmm9
	vpackssdw ymm11, ymm11, ymm13
	vpackssdw xmm26, xmm26, xmm23
	vpackssdw ymm28, ymm19, ymm19
	vpackssdw xmm20, xmm25, xmm30
	vpackssdw ymm23, ymm18, ymm25
	vpackssdw xmm6{k4}, xmm0, xmm0
	vpackssdw ymm6{k2}, ymm3, yword [0xab5]
	vpackssdw zmm5{k7}, zmm0, zword [0xd1c]
	vpackssdw xmm0{k5}{z}, xmm7, oword [0xa97]
	vpackssdw ymm5{k7}{z}, ymm1, ymm6
	vpackssdw zmm3{k3}{z}, zmm0, zmm3
	vpackssdw xmm4, xmm7, oword [eax+1]
	vpackssdw xmm1, xmm4, oword [eax+64]
	vpackssdw ymm3, ymm7, yword [eax+1]
	vpackssdw ymm4, ymm6, yword [eax+64]
	vpackssdw xmm1, xmm1, oword [eax+1]
	vpackssdw xmm1, xmm2, oword [eax+64]
	vpackssdw ymm2, ymm7, yword [eax+1]
	vpackssdw ymm6, ymm0, yword [eax+64]
	vpackssdw zmm1, zmm0, zword [eax+1]
	vpackssdw zmm3, zmm0, zword [eax+64]
	vpackssdw xmm1, xmm7, [0x20e]
	vpackssdw ymm3, ymm7, [0x1db]
	vpackssdw xmm4, xmm1, [0x54a]
	vpackssdw ymm3, ymm4, [0x5b7]
	vpackssdw zmm2, zmm4, [0xd75]
