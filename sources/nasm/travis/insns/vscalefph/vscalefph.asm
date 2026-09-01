default rel
	vscalefph xmm2, xmm3, oword [0xeb9]
	vscalefph xmm2, oword [0xeb9]
	vscalefph xmm6, xmm5, xmm5
	vscalefph ymm3, ymm0, ymm7
	vscalefph ymm3, ymm7
	vscalefph ymm0, ymm0, ymm2
	vscalefph zmm3, zmm7, zword [0x437]
	vscalefph zmm3, zword [0x437]
	vscalefph zmm5, zmm0, zword [0x84c]
	vscalefph xmm14, xmm14, xmm11
	vscalefph ymm11, ymm9, ymm13
	vscalefph zmm9, zmm15, zmm13
	vscalefph xmm21, xmm19, xmm23
	vscalefph ymm17, ymm29, ymm23
	vscalefph zmm23, zmm20, zmm17
	vscalefph xmm6{k6}, xmm4, oword [0x2c3]
	vscalefph ymm0{k7}, ymm7, ymm1
	vscalefph zmm1{k4}, zmm2, zmm0
	vscalefph xmm2{k3}{z}, xmm4, xmm0
	vscalefph ymm5{k4}{z}, ymm4, ymm5
	vscalefph zmm5{k4}{z}, zmm3, zmm4
	vscalefph zmm6, zmm5, zmm2, {ru-sae}
	vscalefph xmm2, xmm3, oword [eax+1]
	vscalefph xmm4, xmm1, oword [eax+64]
	vscalefph ymm1, ymm1, yword [eax+1]
	vscalefph ymm4, ymm2, yword [eax+64]
	vscalefph zmm5, zmm2, zword [eax+1]
	vscalefph zmm3, zmm4, zword [eax+64]
	vscalefph xmm1, xmm2, [0xb97]
	vscalefph ymm4, ymm2, [0xf1d]
	vscalefph zmm7, zmm6, [0xa6b]
