default rel
	vrsqrtph xmm1, xmm0
	vrsqrtph xmm3, xmm5
	vrsqrtph ymm1, ymm4
	vrsqrtph ymm5, yword [0x934]
	vrsqrtph zmm4, zmm5
	vrsqrtph zmm1, zmm3
	vrsqrtph xmm13, xmm12
	vrsqrtph ymm13, ymm9
	vrsqrtph zmm9, zmm8
	vrsqrtph xmm31, xmm24
	vrsqrtph ymm31, ymm21
	vrsqrtph zmm16, zmm31
	vrsqrtph xmm2{k6}, oword [0x76d]
	vrsqrtph ymm3{k5}, ymm0
	vrsqrtph zmm2{k1}, zmm4
	vrsqrtph xmm0{k5}{z}, xmm1
	vrsqrtph ymm0{k1}{z}, ymm0
	vrsqrtph zmm2{k3}{z}, zword [0x783]
	vrsqrtph zmm6, zmm5, {sae}
	vrsqrtph xmm7, oword [eax+1]
	vrsqrtph xmm5, oword [eax+64]
	vrsqrtph ymm2, yword [eax+1]
	vrsqrtph ymm3, yword [eax+64]
	vrsqrtph zmm1, zword [eax+1]
	vrsqrtph zmm0, zword [eax+64]
	vrsqrtph xmm2, [0x1b8]
	vrsqrtph ymm1, [0x147]
	vrsqrtph zmm0, [0x412]
