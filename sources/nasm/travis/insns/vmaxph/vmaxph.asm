default rel
	vmaxph xmm6, xmm4, oword [0xdb4]
	vmaxph xmm6, oword [0xdb4]
	vmaxph xmm6, xmm2, oword [0x671]
	vmaxph ymm6, ymm4, yword [0x279]
	vmaxph ymm6, yword [0x279]
	vmaxph ymm5, ymm0, ymm6
	vmaxph zmm4, zmm3, zmm7
	vmaxph zmm4, zmm7
	vmaxph zmm6, zmm4, zmm0
	vmaxph xmm10, xmm15, xmm8
	vmaxph ymm14, ymm9, ymm12
	vmaxph zmm15, zmm14, zmm8
	vmaxph xmm22, xmm16, xmm19
	vmaxph ymm18, ymm30, ymm17
	vmaxph zmm17, zmm20, zmm28
	vmaxph xmm7{k6}, xmm5, oword [0x703]
	vmaxph ymm2{k2}, ymm3, ymm2
	vmaxph zmm7{k5}, zmm3, zword [0x1d6]
	vmaxph xmm5{k1}{z}, xmm0, xmm7
	vmaxph ymm6{k7}{z}, ymm3, ymm1
	vmaxph zmm0{k4}{z}, zmm2, zmm6
	vmaxph zmm2, zmm4, zmm0, {sae}
	vmaxph xmm2, xmm0, oword [eax+1]
	vmaxph xmm2, xmm0, oword [eax+64]
	vmaxph ymm5, ymm7, yword [eax+1]
	vmaxph ymm6, ymm2, yword [eax+64]
	vmaxph zmm1, zmm0, zword [eax+1]
	vmaxph zmm0, zmm2, zword [eax+64]
	vmaxph xmm3, xmm1, [0x67c]
	vmaxph ymm6, ymm1, [0x2e7]
	vmaxph zmm0, zmm0, [0x337]
