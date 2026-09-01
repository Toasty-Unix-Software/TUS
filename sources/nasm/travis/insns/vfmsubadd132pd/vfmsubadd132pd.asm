default rel
	vfmsubadd132pd xmm3, xmm3, oword [0xa66]
	vfmsubadd132pd xmm7, xmm4, xmm2
	vfmsubadd132pd ymm4, ymm6, ymm4
	vfmsubadd132pd ymm7, ymm1, ymm2
	vfmsubadd132pd xmm7, xmm3, oword [0xd04]
	vfmsubadd132pd xmm7, xmm4, xmm1
	vfmsubadd132pd ymm6, ymm2, ymm3
	vfmsubadd132pd ymm1, ymm2, ymm0
	vfmsubadd132pd xmm9, xmm13, xmm12
	vfmsubadd132pd ymm15, ymm13, ymm12
	vfmsubadd132pd xmm14, xmm13, xmm13
	vfmsubadd132pd ymm12, ymm13, ymm14
	vfmsubadd132pd xmm20, xmm23, xmm19
	vfmsubadd132pd ymm17, ymm28, ymm23
	vfmsubadd132pd xmm22, xmm26, xmm23
	vfmsubadd132pd ymm25, ymm30, ymm23
	vfmsubadd132pd xmm1{k3}, xmm3, xmm4
	vfmsubadd132pd ymm2{k5}, ymm7, ymm0
	vfmsubadd132pd zmm5{k4}, zmm0, zmm6
	vfmsubadd132pd xmm5{k6}{z}, xmm4, xmm6
	vfmsubadd132pd ymm5{k5}{z}, ymm1, ymm7
	vfmsubadd132pd zmm6{k7}{z}, zmm2, zmm7
	vfmsubadd132pd zmm1, zmm1, zmm2, {rn-sae}
	vfmsubadd132pd xmm4, xmm4, oword [eax+1]
	vfmsubadd132pd xmm1, xmm1, oword [eax+64]
	vfmsubadd132pd ymm6, ymm1, yword [eax+1]
	vfmsubadd132pd ymm7, ymm4, yword [eax+64]
	vfmsubadd132pd xmm0, xmm2, oword [eax+1]
	vfmsubadd132pd xmm2, xmm7, oword [eax+64]
	vfmsubadd132pd ymm2, ymm2, yword [eax+1]
	vfmsubadd132pd ymm4, ymm0, yword [eax+64]
	vfmsubadd132pd zmm1, zmm1, zword [eax+1]
	vfmsubadd132pd zmm2, zmm1, zword [eax+64]
	vfmsubadd132pd xmm1, xmm4, [0xdf0]
	vfmsubadd132pd ymm4, ymm5, [0x5ee]
	vfmsubadd132pd xmm2, xmm2, [0x66a]
	vfmsubadd132pd ymm4, ymm3, [0x8e7]
	vfmsubadd132pd zmm2, zmm3, [0x170]
