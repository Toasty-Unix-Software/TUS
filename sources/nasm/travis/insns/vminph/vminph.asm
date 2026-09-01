default rel
	vminph xmm4, xmm2, xmm3
	vminph xmm4, xmm3
	vminph xmm5, xmm5, xmm7
	vminph ymm7, ymm1, ymm1
	vminph ymm7, ymm1
	vminph ymm6, ymm3, ymm5
	vminph zmm0, zmm3, zmm5
	vminph zmm0, zmm5
	vminph zmm5, zmm1, zmm2
	vminph xmm9, xmm9, xmm8
	vminph ymm15, ymm13, ymm11
	vminph zmm12, zmm9, zmm9
	vminph xmm19, xmm25, xmm25
	vminph ymm20, ymm29, ymm16
	vminph zmm26, zmm17, zmm29
	vminph xmm5{k6}, xmm6, xmm3
	vminph ymm1{k7}, ymm6, ymm0
	vminph zmm3{k3}, zmm3, zmm2
	vminph xmm7{k1}{z}, xmm7, oword [0xb68]
	vminph ymm6{k1}{z}, ymm7, yword [0xabe]
	vminph zmm7{k1}{z}, zmm2, zmm4
	vminph zmm1, zmm4, zmm0, {sae}
	vminph xmm1, xmm7, oword [eax+1]
	vminph xmm5, xmm5, oword [eax+64]
	vminph ymm1, ymm4, yword [eax+1]
	vminph ymm7, ymm1, yword [eax+64]
	vminph zmm1, zmm3, zword [eax+1]
	vminph zmm2, zmm4, zword [eax+64]
	vminph xmm5, xmm0, [0x1cd]
	vminph ymm4, ymm1, [0x40a]
	vminph zmm6, zmm0, [0x3de]
