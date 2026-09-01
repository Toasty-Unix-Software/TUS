default rel
	vfcmulcph xmm6, xmm3, xmm3
	vfcmulcph xmm6, xmm3
	vfcmulcph xmm6, xmm7, xmm6
	vfcmulcph ymm3, ymm5, ymm3
	vfcmulcph ymm3, ymm3
	vfcmulcph ymm7, ymm7, yword [0xb9f]
	vfcmulcph zmm1, zmm2, zmm0
	vfcmulcph zmm1, zmm0
	vfcmulcph zmm1, zmm1, zword [0x76c]
	vfcmulcph xmm13, xmm8, xmm8
	vfcmulcph ymm15, ymm8, ymm14
	vfcmulcph zmm14, zmm13, zmm12
	vfcmulcph xmm21, xmm26, xmm18
	vfcmulcph ymm26, ymm16, ymm25
	vfcmulcph zmm31, zmm23, zmm18
	vfcmulcph xmm1{k5}, xmm2, oword [0x681]
	vfcmulcph ymm0{k6}, ymm2, ymm3
	vfcmulcph zmm6{k7}, zmm5, zmm4
	vfcmulcph xmm4{k3}{z}, xmm6, xmm2
	vfcmulcph ymm3{k6}{z}, ymm6, yword [0x7fd]
	vfcmulcph zmm0{k2}{z}, zmm6, zword [0x23e]
	vfcmulcph zmm2, zmm3, zmm6, {rd-sae}
	vfcmulcph xmm0, xmm3, oword [eax+1]
	vfcmulcph xmm2, xmm3, oword [eax+64]
	vfcmulcph ymm3, ymm1, yword [eax+1]
	vfcmulcph ymm6, ymm5, yword [eax+64]
	vfcmulcph zmm2, zmm1, zword [eax+1]
	vfcmulcph zmm5, zmm6, zword [eax+64]
	vfcmulcph xmm2, xmm3, [0x617]
	vfcmulcph ymm0, ymm4, [0xc73]
	vfcmulcph zmm7, zmm5, [0x6d2]
