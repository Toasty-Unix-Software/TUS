default rel
	vaddph xmm0, xmm1, oword [0x2cc]
	vaddph xmm0, oword [0x2cc]
	vaddph xmm6, xmm2, xmm5
	vaddph ymm7, ymm0, yword [0x2ea]
	vaddph ymm7, yword [0x2ea]
	vaddph ymm4, ymm0, ymm2
	vaddph zmm5, zmm6, zmm4
	vaddph zmm5, zmm4
	vaddph zmm4, zmm7, zmm1
	vaddph xmm14, xmm13, xmm9
	vaddph ymm9, ymm9, ymm14
	vaddph zmm9, zmm10, zmm9
	vaddph xmm20, xmm22, xmm26
	vaddph ymm26, ymm20, ymm16
	vaddph zmm20, zmm28, zmm22
	vaddph xmm2{k4}, xmm5, xmm6
	vaddph ymm6{k4}, ymm1, yword [0x6e9]
	vaddph zmm3{k5}, zmm3, zmm5
	vaddph xmm3{k6}{z}, xmm4, xmm5
	vaddph ymm1{k4}{z}, ymm4, ymm3
	vaddph zmm5{k5}{z}, zmm3, zword [0xa89]
	vaddph zmm3, zmm4, zmm6, {rd-sae}
	vaddph xmm6, xmm4, oword [eax+1]
	vaddph xmm0, xmm2, oword [eax+64]
	vaddph ymm7, ymm7, yword [eax+1]
	vaddph ymm6, ymm2, yword [eax+64]
	vaddph zmm2, zmm3, zword [eax+1]
	vaddph zmm4, zmm1, zword [eax+64]
	vaddph xmm6, xmm7, [0xfd0]
	vaddph ymm6, ymm1, [0x35f]
	vaddph zmm6, zmm0, [0x9a3]
