default rel
	vfmaddsub231ph xmm3, xmm6, xmm4
	vfmaddsub231ph xmm3, xmm4
	vfmaddsub231ph xmm6, xmm6, oword [0x426]
	vfmaddsub231ph ymm3, ymm7, ymm1
	vfmaddsub231ph ymm3, ymm1
	vfmaddsub231ph ymm1, ymm3, ymm3
	vfmaddsub231ph zmm0, zmm4, zmm1
	vfmaddsub231ph zmm0, zmm1
	vfmaddsub231ph zmm3, zmm1, zword [0xf00]
	vfmaddsub231ph xmm14, xmm12, xmm11
	vfmaddsub231ph ymm9, ymm11, ymm12
	vfmaddsub231ph zmm13, zmm13, zmm13
	vfmaddsub231ph xmm22, xmm26, xmm21
	vfmaddsub231ph ymm24, ymm30, ymm25
	vfmaddsub231ph zmm22, zmm29, zmm20
	vfmaddsub231ph xmm1{k2}, xmm5, oword [0x1ab]
	vfmaddsub231ph ymm5{k4}, ymm3, ymm3
	vfmaddsub231ph zmm2{k5}, zmm5, zmm5
	vfmaddsub231ph xmm2{k3}{z}, xmm5, oword [0x6ce]
	vfmaddsub231ph ymm5{k7}{z}, ymm1, ymm7
	vfmaddsub231ph zmm0{k3}{z}, zmm5, zmm0
	vfmaddsub231ph zmm6, zmm3, zmm4, {rd-sae}
	vfmaddsub231ph xmm1, xmm3, oword [eax+1]
	vfmaddsub231ph xmm0, xmm1, oword [eax+64]
	vfmaddsub231ph ymm1, ymm1, yword [eax+1]
	vfmaddsub231ph ymm6, ymm3, yword [eax+64]
	vfmaddsub231ph zmm7, zmm1, zword [eax+1]
	vfmaddsub231ph zmm7, zmm6, zword [eax+64]
	vfmaddsub231ph xmm5, xmm2, [0xb7a]
	vfmaddsub231ph ymm5, ymm2, [0x865]
	vfmaddsub231ph zmm5, zmm4, [0xd81]
