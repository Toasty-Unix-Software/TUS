default rel
	vpmuludq xmm1, xmm3, xmm3
	vpmuludq xmm1, xmm3
	vpmuludq xmm7, xmm3, xmm4
	vpmuludq ymm6, ymm2, yword [0x4a0]
	vpmuludq ymm6, yword [0x4a0]
	vpmuludq ymm2, ymm1, ymm3
	vpmuludq xmm0, xmm0, xmm5
	vpmuludq xmm0, xmm5
	vpmuludq xmm3, xmm7, oword [0xa65]
	vpmuludq ymm0, ymm4, ymm2
	vpmuludq ymm0, ymm2
	vpmuludq ymm4, ymm1, ymm0
	vpmuludq xmm10, xmm14, xmm10
	vpmuludq ymm14, ymm12, ymm13
	vpmuludq xmm12, xmm8, xmm11
	vpmuludq ymm15, ymm10, ymm8
	vpmuludq xmm17, xmm19, xmm19
	vpmuludq ymm25, ymm20, ymm21
	vpmuludq xmm24, xmm24, xmm22
	vpmuludq ymm22, ymm25, ymm22
	vpmuludq xmm4{k6}, xmm3, xmm0
	vpmuludq ymm2{k4}, ymm1, yword [0xc0f]
	vpmuludq zmm2{k4}, zmm4, zword [0x2d6]
	vpmuludq xmm7{k2}{z}, xmm7, xmm0
	vpmuludq ymm5{k7}{z}, ymm6, yword [0x248]
	vpmuludq zmm6{k2}{z}, zmm5, zmm5
	vpmuludq xmm6, xmm4, oword [eax+1]
	vpmuludq xmm5, xmm4, oword [eax+64]
	vpmuludq ymm4, ymm6, yword [eax+1]
	vpmuludq ymm2, ymm0, yword [eax+64]
	vpmuludq xmm3, xmm1, oword [eax+1]
	vpmuludq xmm2, xmm3, oword [eax+64]
	vpmuludq ymm5, ymm6, yword [eax+1]
	vpmuludq ymm5, ymm6, yword [eax+64]
	vpmuludq zmm0, zmm0, zword [eax+1]
	vpmuludq zmm2, zmm6, zword [eax+64]
	vpmuludq xmm0, xmm5, [0x27c]
	vpmuludq ymm0, ymm1, [0x385]
	vpmuludq xmm4, xmm4, [0xad5]
	vpmuludq ymm7, ymm4, [0x98a]
	vpmuludq zmm7, zmm1, [0xd72]
