default rel
	vsqrtph xmm3, oword [0xa87]
	vsqrtph xmm0, oword [0x4a5]
	vsqrtph ymm5, yword [0x253]
	vsqrtph ymm5, ymm5
	vsqrtph zmm7, zmm3
	vsqrtph zmm5, zmm5
	vsqrtph xmm14, xmm10
	vsqrtph ymm15, ymm13
	vsqrtph zmm14, zmm12
	vsqrtph xmm17, xmm31
	vsqrtph ymm22, ymm17
	vsqrtph zmm21, zmm29
	vsqrtph xmm6{k3}, xmm7
	vsqrtph ymm2{k2}, yword [0xcf0]
	vsqrtph zmm0{k1}, zmm0
	vsqrtph xmm2{k3}{z}, xmm5
	vsqrtph ymm5{k7}{z}, ymm0
	vsqrtph zmm0{k1}{z}, zmm6
	vsqrtph zmm3, zmm3, {rz-sae}
	vsqrtph xmm1, oword [eax+1]
	vsqrtph xmm6, oword [eax+64]
	vsqrtph ymm6, yword [eax+1]
	vsqrtph ymm1, yword [eax+64]
	vsqrtph zmm5, zword [eax+1]
	vsqrtph zmm0, zword [eax+64]
	vsqrtph xmm7, [0xe6b]
	vsqrtph ymm0, [0x98e]
	vsqrtph zmm6, [0x1d4]
