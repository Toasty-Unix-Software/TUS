default rel
	vfmaddsub132pd xmm0, xmm6, xmm1
	vfmaddsub132pd xmm4, xmm4, xmm2
	vfmaddsub132pd ymm7, ymm0, yword [0x2e9]
	vfmaddsub132pd ymm6, ymm6, yword [0xb2f]
	vfmaddsub132pd xmm6, xmm2, oword [0xae6]
	vfmaddsub132pd xmm7, xmm4, xmm7
	vfmaddsub132pd ymm4, ymm7, ymm4
	vfmaddsub132pd ymm4, ymm7, ymm2
	vfmaddsub132pd xmm14, xmm15, xmm12
	vfmaddsub132pd ymm11, ymm12, ymm9
	vfmaddsub132pd xmm13, xmm15, xmm8
	vfmaddsub132pd ymm11, ymm9, ymm8
	vfmaddsub132pd xmm16, xmm27, xmm25
	vfmaddsub132pd ymm26, ymm19, ymm28
	vfmaddsub132pd xmm17, xmm27, xmm17
	vfmaddsub132pd ymm23, ymm25, ymm17
	vfmaddsub132pd xmm4{k1}, xmm3, xmm3
	vfmaddsub132pd ymm2{k7}, ymm2, ymm1
	vfmaddsub132pd zmm7{k7}, zmm1, zmm6
	vfmaddsub132pd xmm3{k7}{z}, xmm2, xmm2
	vfmaddsub132pd ymm6{k5}{z}, ymm2, yword [0x3be]
	vfmaddsub132pd zmm5{k3}{z}, zmm2, zmm6
	vfmaddsub132pd zmm0, zmm6, zmm5, {rz-sae}
	vfmaddsub132pd xmm7, xmm2, oword [eax+1]
	vfmaddsub132pd xmm5, xmm7, oword [eax+64]
	vfmaddsub132pd ymm0, ymm5, yword [eax+1]
	vfmaddsub132pd ymm3, ymm2, yword [eax+64]
	vfmaddsub132pd xmm4, xmm0, oword [eax+1]
	vfmaddsub132pd xmm1, xmm2, oword [eax+64]
	vfmaddsub132pd ymm2, ymm5, yword [eax+1]
	vfmaddsub132pd ymm6, ymm7, yword [eax+64]
	vfmaddsub132pd zmm5, zmm7, zword [eax+1]
	vfmaddsub132pd zmm5, zmm7, zword [eax+64]
	vfmaddsub132pd xmm7, xmm1, [0xc21]
	vfmaddsub132pd ymm6, ymm4, [0x146]
	vfmaddsub132pd xmm6, xmm5, [0xbf5]
	vfmaddsub132pd ymm4, ymm7, [0x5b4]
	vfmaddsub132pd zmm6, zmm2, [0x7df]
