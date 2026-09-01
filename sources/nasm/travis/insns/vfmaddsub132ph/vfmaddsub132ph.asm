default rel
	vfmaddsub132ph xmm4, xmm7, xmm0
	vfmaddsub132ph xmm4, xmm0
	vfmaddsub132ph xmm1, xmm7, xmm6
	vfmaddsub132ph ymm0, ymm7, ymm5
	vfmaddsub132ph ymm0, ymm5
	vfmaddsub132ph ymm3, ymm5, yword [0xd6d]
	vfmaddsub132ph zmm6, zmm4, zword [0xa0b]
	vfmaddsub132ph zmm6, zword [0xa0b]
	vfmaddsub132ph zmm6, zmm6, zmm6
	vfmaddsub132ph xmm11, xmm8, xmm15
	vfmaddsub132ph ymm15, ymm15, ymm9
	vfmaddsub132ph zmm13, zmm9, zmm11
	vfmaddsub132ph xmm24, xmm26, xmm30
	vfmaddsub132ph ymm21, ymm27, ymm29
	vfmaddsub132ph zmm20, zmm22, zmm22
	vfmaddsub132ph xmm5{k5}, xmm3, xmm4
	vfmaddsub132ph ymm2{k3}, ymm6, yword [0x43f]
	vfmaddsub132ph zmm0{k1}, zmm0, zmm7
	vfmaddsub132ph xmm3{k2}{z}, xmm2, xmm3
	vfmaddsub132ph ymm0{k2}{z}, ymm4, yword [0x80f]
	vfmaddsub132ph zmm1{k7}{z}, zmm5, zmm5
	vfmaddsub132ph zmm4, zmm3, zmm1, {rn-sae}
	vfmaddsub132ph xmm3, xmm0, oword [eax+1]
	vfmaddsub132ph xmm1, xmm5, oword [eax+64]
	vfmaddsub132ph ymm2, ymm5, yword [eax+1]
	vfmaddsub132ph ymm7, ymm0, yword [eax+64]
	vfmaddsub132ph zmm4, zmm3, zword [eax+1]
	vfmaddsub132ph zmm4, zmm2, zword [eax+64]
	vfmaddsub132ph xmm0, xmm2, [0x19d]
	vfmaddsub132ph ymm5, ymm4, [0x462]
	vfmaddsub132ph zmm1, zmm4, [0xee4]
