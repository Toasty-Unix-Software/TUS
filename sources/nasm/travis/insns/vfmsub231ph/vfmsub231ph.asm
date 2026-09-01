default rel
	vfmsub231ph xmm4, xmm6, xmm2
	vfmsub231ph xmm4, xmm2
	vfmsub231ph xmm1, xmm4, xmm3
	vfmsub231ph ymm2, ymm4, ymm3
	vfmsub231ph ymm2, ymm3
	vfmsub231ph ymm2, ymm5, yword [0xde6]
	vfmsub231ph zmm7, zmm2, zmm1
	vfmsub231ph zmm7, zmm1
	vfmsub231ph zmm0, zmm3, zmm4
	vfmsub231ph xmm15, xmm14, xmm8
	vfmsub231ph ymm10, ymm10, ymm14
	vfmsub231ph zmm13, zmm13, zmm14
	vfmsub231ph xmm16, xmm29, xmm20
	vfmsub231ph ymm17, ymm26, ymm29
	vfmsub231ph zmm26, zmm19, zmm28
	vfmsub231ph xmm2{k3}, xmm3, xmm5
	vfmsub231ph ymm2{k2}, ymm4, yword [0x608]
	vfmsub231ph zmm3{k1}, zmm1, zmm1
	vfmsub231ph xmm7{k5}{z}, xmm3, oword [0x76a]
	vfmsub231ph ymm2{k3}{z}, ymm1, ymm4
	vfmsub231ph zmm7{k7}{z}, zmm1, zword [0xd42]
	vfmsub231ph zmm3, zmm2, zmm6, {rd-sae}
	vfmsub231ph xmm6, xmm4, oword [eax+1]
	vfmsub231ph xmm6, xmm7, oword [eax+64]
	vfmsub231ph ymm0, ymm0, yword [eax+1]
	vfmsub231ph ymm1, ymm6, yword [eax+64]
	vfmsub231ph zmm2, zmm6, zword [eax+1]
	vfmsub231ph zmm1, zmm2, zword [eax+64]
	vfmsub231ph xmm4, xmm4, [0x10b]
	vfmsub231ph ymm4, ymm7, [0x81a]
	vfmsub231ph zmm5, zmm2, [0xe3e]
