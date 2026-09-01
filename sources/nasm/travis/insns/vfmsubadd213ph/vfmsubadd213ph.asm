default rel
	vfmsubadd213ph xmm4, xmm7, xmm4
	vfmsubadd213ph xmm4, xmm4
	vfmsubadd213ph xmm4, xmm0, xmm3
	vfmsubadd213ph ymm6, ymm0, yword [0x166]
	vfmsubadd213ph ymm6, yword [0x166]
	vfmsubadd213ph ymm3, ymm2, yword [0xd35]
	vfmsubadd213ph zmm6, zmm4, zmm7
	vfmsubadd213ph zmm6, zmm7
	vfmsubadd213ph zmm5, zmm3, zword [0x621]
	vfmsubadd213ph xmm10, xmm10, xmm8
	vfmsubadd213ph ymm11, ymm11, ymm14
	vfmsubadd213ph zmm14, zmm10, zmm9
	vfmsubadd213ph xmm19, xmm23, xmm29
	vfmsubadd213ph ymm24, ymm31, ymm26
	vfmsubadd213ph zmm18, zmm20, zmm24
	vfmsubadd213ph xmm3{k2}, xmm0, oword [0xa48]
	vfmsubadd213ph ymm3{k7}, ymm2, ymm4
	vfmsubadd213ph zmm4{k3}, zmm3, zmm2
	vfmsubadd213ph xmm6{k1}{z}, xmm1, xmm3
	vfmsubadd213ph ymm3{k3}{z}, ymm2, ymm2
	vfmsubadd213ph zmm6{k6}{z}, zmm5, zword [0x7f0]
	vfmsubadd213ph zmm7, zmm2, zmm5, {ru-sae}
	vfmsubadd213ph xmm1, xmm5, oword [eax+1]
	vfmsubadd213ph xmm3, xmm1, oword [eax+64]
	vfmsubadd213ph ymm5, ymm0, yword [eax+1]
	vfmsubadd213ph ymm1, ymm3, yword [eax+64]
	vfmsubadd213ph zmm5, zmm1, zword [eax+1]
	vfmsubadd213ph zmm6, zmm7, zword [eax+64]
	vfmsubadd213ph xmm0, xmm5, [0xda8]
	vfmsubadd213ph ymm5, ymm0, [0x970]
	vfmsubadd213ph zmm1, zmm4, [0x7d7]
