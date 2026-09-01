default rel
	vfcmaddcph xmm3, xmm3, xmm0
	vfcmaddcph xmm3, xmm0
	vfcmaddcph xmm3, xmm1, oword [0x588]
	vfcmaddcph ymm5, ymm2, ymm4
	vfcmaddcph ymm5, ymm4
	vfcmaddcph ymm0, ymm4, ymm3
	vfcmaddcph zmm6, zmm2, zmm1
	vfcmaddcph zmm6, zmm1
	vfcmaddcph zmm5, zmm7, zmm0
	vfcmaddcph xmm8, xmm9, xmm13
	vfcmaddcph ymm11, ymm8, ymm9
	vfcmaddcph zmm8, zmm8, zmm13
	vfcmaddcph xmm23, xmm30, xmm26
	vfcmaddcph ymm20, ymm25, ymm26
	vfcmaddcph zmm30, zmm26, zmm27
	vfcmaddcph xmm6{k3}, xmm0, xmm6
	vfcmaddcph ymm0{k1}, ymm0, ymm2
	vfcmaddcph zmm7{k3}, zmm5, zword [0x61e]
	vfcmaddcph xmm7{k5}{z}, xmm3, oword [0xbcf]
	vfcmaddcph ymm0{k5}{z}, ymm0, ymm1
	vfcmaddcph zmm0{k2}{z}, zmm6, zmm2
	vfcmaddcph zmm6, zmm4, zmm0, {rd-sae}
	vfcmaddcph xmm1, xmm4, oword [eax+1]
	vfcmaddcph xmm7, xmm1, oword [eax+64]
	vfcmaddcph ymm0, ymm2, yword [eax+1]
	vfcmaddcph ymm6, ymm2, yword [eax+64]
	vfcmaddcph zmm6, zmm3, zword [eax+1]
	vfcmaddcph zmm3, zmm5, zword [eax+64]
	vfcmaddcph xmm0, xmm2, [0xdff]
	vfcmaddcph ymm3, ymm1, [0xa28]
	vfcmaddcph zmm6, zmm0, [0x9b9]
