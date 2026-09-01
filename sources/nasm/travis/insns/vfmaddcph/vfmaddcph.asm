default rel
	vfmaddcph xmm2, xmm6, oword [0x451]
	vfmaddcph xmm2, oword [0x451]
	vfmaddcph xmm5, xmm6, xmm4
	vfmaddcph ymm1, ymm0, ymm1
	vfmaddcph ymm1, ymm1
	vfmaddcph ymm5, ymm5, ymm2
	vfmaddcph zmm6, zmm3, zmm7
	vfmaddcph zmm6, zmm7
	vfmaddcph zmm0, zmm6, zmm5
	vfmaddcph xmm8, xmm12, xmm14
	vfmaddcph ymm9, ymm14, ymm8
	vfmaddcph zmm8, zmm10, zmm10
	vfmaddcph xmm25, xmm21, xmm28
	vfmaddcph ymm26, ymm31, ymm26
	vfmaddcph zmm19, zmm18, zmm31
	vfmaddcph xmm6{k5}, xmm1, xmm7
	vfmaddcph ymm6{k3}, ymm3, yword [0x7db]
	vfmaddcph zmm1{k1}, zmm0, zmm3
	vfmaddcph xmm6{k7}{z}, xmm6, xmm7
	vfmaddcph ymm3{k1}{z}, ymm6, ymm7
	vfmaddcph zmm4{k3}{z}, zmm0, zword [0x1e8]
	vfmaddcph zmm3, zmm7, zmm2, {rn-sae}
	vfmaddcph xmm6, xmm0, oword [eax+1]
	vfmaddcph xmm1, xmm2, oword [eax+64]
	vfmaddcph ymm4, ymm2, yword [eax+1]
	vfmaddcph ymm0, ymm1, yword [eax+64]
	vfmaddcph zmm3, zmm3, zword [eax+1]
	vfmaddcph zmm3, zmm3, zword [eax+64]
	vfmaddcph xmm6, xmm4, [0x49f]
	vfmaddcph ymm7, ymm6, [0x4ef]
	vfmaddcph zmm2, zmm4, [0x9d9]
