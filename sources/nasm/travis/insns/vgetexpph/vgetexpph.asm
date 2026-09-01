default rel
	vgetexpph xmm4, xmm4
	vgetexpph xmm5, xmm3
	vgetexpph ymm3, yword [0x5ac]
	vgetexpph ymm0, ymm4
	vgetexpph zmm7, zword [0x69d]
	vgetexpph zmm6, zword [0x397]
	vgetexpph xmm13, xmm12
	vgetexpph ymm8, ymm14
	vgetexpph zmm8, zmm8
	vgetexpph xmm26, xmm17
	vgetexpph ymm22, ymm21
	vgetexpph zmm22, zmm22
	vgetexpph xmm5{k3}, xmm4
	vgetexpph ymm1{k5}, yword [0x38c]
	vgetexpph zmm5{k1}, zword [0x6ac]
	vgetexpph xmm1{k4}{z}, xmm7
	vgetexpph ymm5{k4}{z}, ymm4
	vgetexpph zmm6{k4}{z}, zword [0x68f]
	vgetexpph zmm1, zmm7, {sae}
	vgetexpph xmm5, oword [eax+1]
	vgetexpph xmm1, oword [eax+64]
	vgetexpph ymm4, yword [eax+1]
	vgetexpph ymm7, yword [eax+64]
	vgetexpph zmm3, zword [eax+1]
	vgetexpph zmm2, zword [eax+64]
	vgetexpph xmm2, [0x395]
	vgetexpph ymm1, [0xc0f]
	vgetexpph zmm1, [0xc0b]
