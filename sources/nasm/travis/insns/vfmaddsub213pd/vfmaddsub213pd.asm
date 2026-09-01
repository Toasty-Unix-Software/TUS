default rel
	vfmaddsub213pd xmm4, xmm4, xmm4
	vfmaddsub213pd xmm4, xmm4, xmm2
	vfmaddsub213pd ymm5, ymm5, ymm1
	vfmaddsub213pd ymm0, ymm7, yword [0x2e2]
	vfmaddsub213pd xmm2, xmm0, oword [0xbbc]
	vfmaddsub213pd xmm1, xmm7, xmm6
	vfmaddsub213pd ymm6, ymm3, yword [0xc9e]
	vfmaddsub213pd ymm7, ymm0, ymm6
	vfmaddsub213pd xmm10, xmm11, xmm13
	vfmaddsub213pd ymm9, ymm12, ymm9
	vfmaddsub213pd xmm13, xmm15, xmm10
	vfmaddsub213pd ymm15, ymm9, ymm12
	vfmaddsub213pd xmm31, xmm27, xmm19
	vfmaddsub213pd ymm17, ymm23, ymm21
	vfmaddsub213pd xmm28, xmm30, xmm29
	vfmaddsub213pd ymm16, ymm23, ymm17
	vfmaddsub213pd xmm1{k2}, xmm5, xmm3
	vfmaddsub213pd ymm6{k4}, ymm5, ymm6
	vfmaddsub213pd zmm0{k6}, zmm4, zmm4
	vfmaddsub213pd xmm5{k4}{z}, xmm5, xmm5
	vfmaddsub213pd ymm3{k1}{z}, ymm1, ymm4
	vfmaddsub213pd zmm5{k1}{z}, zmm1, zmm6
	vfmaddsub213pd zmm3, zmm2, zmm5, {rz-sae}
	vfmaddsub213pd xmm3, xmm5, oword [eax+1]
	vfmaddsub213pd xmm6, xmm5, oword [eax+64]
	vfmaddsub213pd ymm3, ymm0, yword [eax+1]
	vfmaddsub213pd ymm3, ymm5, yword [eax+64]
	vfmaddsub213pd xmm0, xmm7, oword [eax+1]
	vfmaddsub213pd xmm5, xmm5, oword [eax+64]
	vfmaddsub213pd ymm2, ymm6, yword [eax+1]
	vfmaddsub213pd ymm5, ymm7, yword [eax+64]
	vfmaddsub213pd zmm6, zmm5, zword [eax+1]
	vfmaddsub213pd zmm1, zmm6, zword [eax+64]
	vfmaddsub213pd xmm2, xmm6, [0xbc0]
	vfmaddsub213pd ymm7, ymm3, [0x77d]
	vfmaddsub213pd xmm5, xmm5, [0x56a]
	vfmaddsub213pd ymm3, ymm4, [0x6a7]
	vfmaddsub213pd zmm3, zmm2, [0x298]
