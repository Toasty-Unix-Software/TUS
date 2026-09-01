default rel
	vfmsubadd132ph xmm4, xmm2, xmm4
	vfmsubadd132ph xmm4, xmm4
	vfmsubadd132ph xmm7, xmm7, oword [0xa66]
	vfmsubadd132ph ymm7, ymm2, yword [0x183]
	vfmsubadd132ph ymm7, yword [0x183]
	vfmsubadd132ph ymm2, ymm0, ymm3
	vfmsubadd132ph zmm5, zmm0, zmm2
	vfmsubadd132ph zmm5, zmm2
	vfmsubadd132ph zmm0, zmm7, zmm1
	vfmsubadd132ph xmm15, xmm14, xmm11
	vfmsubadd132ph ymm10, ymm9, ymm12
	vfmsubadd132ph zmm8, zmm14, zmm10
	vfmsubadd132ph xmm24, xmm24, xmm22
	vfmsubadd132ph ymm28, ymm19, ymm25
	vfmsubadd132ph zmm29, zmm31, zmm25
	vfmsubadd132ph xmm7{k6}, xmm7, oword [0x5e1]
	vfmsubadd132ph ymm2{k3}, ymm7, ymm6
	vfmsubadd132ph zmm1{k4}, zmm4, zword [0x81c]
	vfmsubadd132ph xmm1{k5}{z}, xmm1, oword [0x98f]
	vfmsubadd132ph ymm5{k6}{z}, ymm4, yword [0x81b]
	vfmsubadd132ph zmm2{k2}{z}, zmm6, zmm2
	vfmsubadd132ph zmm0, zmm0, zmm4, {rd-sae}
	vfmsubadd132ph xmm2, xmm4, oword [eax+1]
	vfmsubadd132ph xmm2, xmm1, oword [eax+64]
	vfmsubadd132ph ymm1, ymm1, yword [eax+1]
	vfmsubadd132ph ymm6, ymm4, yword [eax+64]
	vfmsubadd132ph zmm6, zmm6, zword [eax+1]
	vfmsubadd132ph zmm4, zmm3, zword [eax+64]
	vfmsubadd132ph xmm1, xmm7, [0x8fe]
	vfmsubadd132ph ymm2, ymm4, [0x1b5]
	vfmsubadd132ph zmm7, zmm7, [0x7a1]
