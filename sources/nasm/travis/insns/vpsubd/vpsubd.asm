default rel
	vpsubd xmm4, xmm2, oword [0xbf2]
	vpsubd xmm4, oword [0xbf2]
	vpsubd xmm7, xmm1, xmm5
	vpsubd ymm5, ymm4, yword [0xa69]
	vpsubd ymm5, yword [0xa69]
	vpsubd ymm1, ymm1, ymm0
	vpsubd xmm2, xmm2, xmm0
	vpsubd xmm2, xmm0
	vpsubd xmm0, xmm0, xmm3
	vpsubd ymm6, ymm7, yword [0xf5d]
	vpsubd ymm6, yword [0xf5d]
	vpsubd ymm0, ymm0, yword [0x56e]
	vpsubd xmm10, xmm12, xmm12
	vpsubd ymm12, ymm14, ymm8
	vpsubd xmm9, xmm15, xmm8
	vpsubd ymm12, ymm12, ymm15
	vpsubd xmm23, xmm24, xmm30
	vpsubd ymm17, ymm16, ymm29
	vpsubd xmm19, xmm25, xmm23
	vpsubd ymm31, ymm27, ymm27
	vpsubd xmm6{k3}, xmm3, xmm0
	vpsubd ymm7{k6}, ymm0, yword [0x6c4]
	vpsubd zmm5{k7}, zmm0, zmm7
	vpsubd xmm3{k2}{z}, xmm6, xmm1
	vpsubd ymm5{k4}{z}, ymm2, ymm4
	vpsubd zmm2{k2}{z}, zmm0, zmm2
	vpsubd xmm6, xmm7, oword [eax+1]
	vpsubd xmm6, xmm1, oword [eax+64]
	vpsubd ymm5, ymm7, yword [eax+1]
	vpsubd ymm0, ymm0, yword [eax+64]
	vpsubd xmm1, xmm6, oword [eax+1]
	vpsubd xmm3, xmm1, oword [eax+64]
	vpsubd ymm7, ymm7, yword [eax+1]
	vpsubd ymm1, ymm0, yword [eax+64]
	vpsubd zmm1, zmm3, zword [eax+1]
	vpsubd zmm0, zmm1, zword [eax+64]
	vpsubd xmm1, xmm0, [0x921]
	vpsubd ymm5, ymm3, [0x85b]
	vpsubd xmm0, xmm7, [0xd71]
	vpsubd ymm0, ymm4, [0x1c3]
	vpsubd zmm2, zmm1, [0xa7f]
