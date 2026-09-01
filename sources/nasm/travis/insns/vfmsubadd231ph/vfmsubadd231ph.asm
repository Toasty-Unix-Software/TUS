default rel
	vfmsubadd231ph xmm6, xmm0, xmm3
	vfmsubadd231ph xmm6, xmm3
	vfmsubadd231ph xmm7, xmm6, oword [0x232]
	vfmsubadd231ph ymm2, ymm6, yword [0x282]
	vfmsubadd231ph ymm2, yword [0x282]
	vfmsubadd231ph ymm1, ymm7, ymm0
	vfmsubadd231ph zmm0, zmm3, zmm1
	vfmsubadd231ph zmm0, zmm1
	vfmsubadd231ph zmm5, zmm0, zword [0x7ce]
	vfmsubadd231ph xmm15, xmm10, xmm9
	vfmsubadd231ph ymm10, ymm14, ymm10
	vfmsubadd231ph zmm8, zmm12, zmm9
	vfmsubadd231ph xmm25, xmm16, xmm25
	vfmsubadd231ph ymm19, ymm16, ymm27
	vfmsubadd231ph zmm27, zmm28, zmm24
	vfmsubadd231ph xmm2{k6}, xmm6, xmm7
	vfmsubadd231ph ymm0{k5}, ymm1, ymm2
	vfmsubadd231ph zmm7{k7}, zmm0, zmm6
	vfmsubadd231ph xmm2{k3}{z}, xmm2, xmm5
	vfmsubadd231ph ymm4{k4}{z}, ymm1, ymm0
	vfmsubadd231ph zmm4{k1}{z}, zmm4, zmm6
	vfmsubadd231ph zmm4, zmm6, zmm5, {rz-sae}
	vfmsubadd231ph xmm7, xmm2, oword [eax+1]
	vfmsubadd231ph xmm6, xmm3, oword [eax+64]
	vfmsubadd231ph ymm7, ymm5, yword [eax+1]
	vfmsubadd231ph ymm6, ymm3, yword [eax+64]
	vfmsubadd231ph zmm3, zmm6, zword [eax+1]
	vfmsubadd231ph zmm0, zmm6, zword [eax+64]
	vfmsubadd231ph xmm4, xmm0, [0x1db]
	vfmsubadd231ph ymm4, ymm3, [0xf49]
	vfmsubadd231ph zmm3, zmm4, [0x396]
