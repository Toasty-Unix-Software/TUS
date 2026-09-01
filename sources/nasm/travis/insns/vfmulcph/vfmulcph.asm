default rel
	vfmulcph xmm4, xmm4, xmm2
	vfmulcph xmm4, xmm2
	vfmulcph xmm6, xmm6, xmm6
	vfmulcph ymm2, ymm5, ymm5
	vfmulcph ymm2, ymm5
	vfmulcph ymm6, ymm3, ymm0
	vfmulcph zmm0, zmm1, zmm5
	vfmulcph zmm0, zmm5
	vfmulcph zmm0, zmm7, zword [0x671]
	vfmulcph xmm10, xmm12, xmm13
	vfmulcph ymm11, ymm10, ymm12
	vfmulcph zmm11, zmm13, zmm8
	vfmulcph xmm30, xmm27, xmm28
	vfmulcph ymm22, ymm18, ymm25
	vfmulcph zmm22, zmm17, zmm26
	vfmulcph xmm5{k2}, xmm6, xmm3
	vfmulcph ymm1{k5}, ymm7, yword [0xdd1]
	vfmulcph zmm1{k6}, zmm0, zword [0x27b]
	vfmulcph xmm6{k5}{z}, xmm2, xmm0
	vfmulcph ymm3{k5}{z}, ymm6, ymm4
	vfmulcph zmm7{k7}{z}, zmm3, zword [0xa5b]
	vfmulcph zmm1, zmm0, zmm4, {rn-sae}
	vfmulcph xmm2, xmm3, oword [eax+1]
	vfmulcph xmm7, xmm2, oword [eax+64]
	vfmulcph ymm3, ymm4, yword [eax+1]
	vfmulcph ymm5, ymm2, yword [eax+64]
	vfmulcph zmm0, zmm5, zword [eax+1]
	vfmulcph zmm3, zmm7, zword [eax+64]
	vfmulcph xmm6, xmm3, [0xf7a]
	vfmulcph ymm0, ymm6, [0xfe5]
	vfmulcph zmm2, zmm3, [0x473]
