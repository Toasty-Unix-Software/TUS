default rel
	vfmaddsub231pd xmm4, xmm1, xmm2
	vfmaddsub231pd xmm0, xmm1, xmm6
	vfmaddsub231pd ymm1, ymm5, yword [0x2ff]
	vfmaddsub231pd ymm5, ymm1, ymm0
	vfmaddsub231pd xmm6, xmm3, xmm5
	vfmaddsub231pd xmm1, xmm6, xmm0
	vfmaddsub231pd ymm3, ymm3, ymm3
	vfmaddsub231pd ymm3, ymm4, ymm0
	vfmaddsub231pd xmm15, xmm11, xmm11
	vfmaddsub231pd ymm13, ymm12, ymm10
	vfmaddsub231pd xmm13, xmm12, xmm12
	vfmaddsub231pd ymm9, ymm10, ymm14
	vfmaddsub231pd xmm18, xmm26, xmm20
	vfmaddsub231pd ymm28, ymm22, ymm31
	vfmaddsub231pd xmm17, xmm29, xmm25
	vfmaddsub231pd ymm16, ymm25, ymm21
	vfmaddsub231pd xmm3{k7}, xmm7, xmm6
	vfmaddsub231pd ymm4{k1}, ymm6, ymm7
	vfmaddsub231pd zmm5{k5}, zmm3, zmm5
	vfmaddsub231pd xmm3{k2}{z}, xmm6, xmm6
	vfmaddsub231pd ymm6{k2}{z}, ymm4, ymm7
	vfmaddsub231pd zmm6{k7}{z}, zmm5, zword [0x936]
	vfmaddsub231pd zmm2, zmm3, zmm2, {rn-sae}
	vfmaddsub231pd xmm2, xmm3, oword [eax+1]
	vfmaddsub231pd xmm4, xmm0, oword [eax+64]
	vfmaddsub231pd ymm7, ymm6, yword [eax+1]
	vfmaddsub231pd ymm1, ymm4, yword [eax+64]
	vfmaddsub231pd xmm1, xmm7, oword [eax+1]
	vfmaddsub231pd xmm4, xmm6, oword [eax+64]
	vfmaddsub231pd ymm4, ymm5, yword [eax+1]
	vfmaddsub231pd ymm0, ymm5, yword [eax+64]
	vfmaddsub231pd zmm1, zmm5, zword [eax+1]
	vfmaddsub231pd zmm6, zmm1, zword [eax+64]
	vfmaddsub231pd xmm3, xmm1, [0xcb8]
	vfmaddsub231pd ymm1, ymm4, [0xca4]
	vfmaddsub231pd xmm1, xmm6, [0xd4c]
	vfmaddsub231pd ymm7, ymm6, [0x5e6]
	vfmaddsub231pd zmm1, zmm4, [0x7f4]
