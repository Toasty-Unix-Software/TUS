default rel
	vsubph xmm6, xmm2, xmm2
	vsubph xmm6, xmm2
	vsubph xmm4, xmm5, oword [0xa80]
	vsubph ymm1, ymm1, yword [0xea5]
	vsubph ymm1, yword [0xea5]
	vsubph ymm2, ymm7, yword [0x482]
	vsubph zmm3, zmm6, zmm4
	vsubph zmm3, zmm4
	vsubph zmm4, zmm5, zmm5
	vsubph xmm9, xmm8, xmm8
	vsubph ymm13, ymm12, ymm13
	vsubph zmm9, zmm10, zmm14
	vsubph xmm28, xmm31, xmm17
	vsubph ymm25, ymm26, ymm16
	vsubph zmm16, zmm26, zmm22
	vsubph xmm2{k7}, xmm0, oword [0x23d]
	vsubph ymm4{k6}, ymm4, ymm6
	vsubph zmm2{k2}, zmm6, zmm0
	vsubph xmm0{k4}{z}, xmm0, xmm2
	vsubph ymm3{k6}{z}, ymm3, ymm2
	vsubph zmm4{k5}{z}, zmm6, zword [0x98d]
	vsubph zmm4, zmm4, zmm1, {ru-sae}
	vsubph xmm3, xmm4, oword [eax+1]
	vsubph xmm3, xmm1, oword [eax+64]
	vsubph ymm0, ymm5, yword [eax+1]
	vsubph ymm3, ymm3, yword [eax+64]
	vsubph zmm2, zmm0, zword [eax+1]
	vsubph zmm2, zmm2, zword [eax+64]
	vsubph xmm7, xmm0, [0x855]
	vsubph ymm3, ymm1, [0x57d]
	vsubph zmm3, zmm4, [0xbcb]
