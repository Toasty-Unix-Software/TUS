default rel
	vfmsub132pd xmm1, xmm7, xmm3
	vfmsub132pd xmm0, xmm3, xmm2
	vfmsub132pd ymm7, ymm3, ymm7
	vfmsub132pd ymm1, ymm5, ymm3
	vfmsub132pd xmm5, xmm1, oword [0x6bb]
	vfmsub132pd xmm0, xmm5, xmm7
	vfmsub132pd ymm5, ymm5, ymm7
	vfmsub132pd ymm1, ymm7, yword [0x283]
	vfmsub132pd xmm10, xmm10, xmm13
	vfmsub132pd ymm8, ymm8, ymm9
	vfmsub132pd xmm13, xmm10, xmm10
	vfmsub132pd ymm15, ymm12, ymm11
	vfmsub132pd xmm19, xmm18, xmm26
	vfmsub132pd ymm29, ymm16, ymm16
	vfmsub132pd xmm22, xmm20, xmm19
	vfmsub132pd ymm19, ymm17, ymm31
	vfmsub132pd xmm4{k5}, xmm7, xmm4
	vfmsub132pd ymm3{k5}, ymm3, ymm5
	vfmsub132pd zmm5{k3}, zmm5, zword [0x203]
	vfmsub132pd xmm1{k2}{z}, xmm1, xmm6
	vfmsub132pd ymm5{k6}{z}, ymm0, ymm1
	vfmsub132pd zmm6{k1}{z}, zmm1, zmm0
	vfmsub132pd zmm3, zmm6, zmm6, {ru-sae}
	vfmsub132pd xmm2, xmm0, oword [eax+1]
	vfmsub132pd xmm7, xmm3, oword [eax+64]
	vfmsub132pd ymm7, ymm4, yword [eax+1]
	vfmsub132pd ymm4, ymm1, yword [eax+64]
	vfmsub132pd xmm4, xmm3, oword [eax+1]
	vfmsub132pd xmm5, xmm2, oword [eax+64]
	vfmsub132pd ymm0, ymm2, yword [eax+1]
	vfmsub132pd ymm6, ymm4, yword [eax+64]
	vfmsub132pd zmm3, zmm1, zword [eax+1]
	vfmsub132pd zmm7, zmm2, zword [eax+64]
	vfmsub132pd xmm0, xmm3, [0xc0b]
	vfmsub132pd ymm6, ymm5, [0x283]
	vfmsub132pd xmm6, xmm4, [0xa78]
	vfmsub132pd ymm4, ymm1, [0xcd1]
	vfmsub132pd zmm0, zmm0, [0x429]
