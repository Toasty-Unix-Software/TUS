default rel
	vfnmadd132pd xmm7, xmm6, xmm4
	vfnmadd132pd xmm0, xmm7, oword [0xdbb]
	vfnmadd132pd ymm4, ymm2, ymm3
	vfnmadd132pd ymm6, ymm5, ymm1
	vfnmadd132pd xmm5, xmm5, xmm0
	vfnmadd132pd xmm2, xmm7, xmm1
	vfnmadd132pd ymm2, ymm3, ymm1
	vfnmadd132pd ymm3, ymm2, yword [0x98c]
	vfnmadd132pd xmm11, xmm9, xmm14
	vfnmadd132pd ymm10, ymm8, ymm9
	vfnmadd132pd xmm8, xmm9, xmm9
	vfnmadd132pd ymm15, ymm12, ymm9
	vfnmadd132pd xmm27, xmm18, xmm30
	vfnmadd132pd ymm17, ymm16, ymm21
	vfnmadd132pd xmm17, xmm29, xmm20
	vfnmadd132pd ymm28, ymm27, ymm19
	vfnmadd132pd xmm5{k6}, xmm0, xmm6
	vfnmadd132pd ymm4{k2}, ymm3, ymm3
	vfnmadd132pd zmm6{k5}, zmm3, zmm0
	vfnmadd132pd xmm7{k5}{z}, xmm5, xmm4
	vfnmadd132pd ymm0{k4}{z}, ymm3, ymm5
	vfnmadd132pd zmm2{k4}{z}, zmm4, zmm0
	vfnmadd132pd zmm7, zmm6, zmm2, {rz-sae}
	vfnmadd132pd xmm6, xmm6, oword [eax+1]
	vfnmadd132pd xmm7, xmm4, oword [eax+64]
	vfnmadd132pd ymm2, ymm2, yword [eax+1]
	vfnmadd132pd ymm0, ymm4, yword [eax+64]
	vfnmadd132pd xmm7, xmm5, oword [eax+1]
	vfnmadd132pd xmm3, xmm7, oword [eax+64]
	vfnmadd132pd ymm6, ymm4, yword [eax+1]
	vfnmadd132pd ymm4, ymm7, yword [eax+64]
	vfnmadd132pd zmm5, zmm7, zword [eax+1]
	vfnmadd132pd zmm7, zmm0, zword [eax+64]
	vfnmadd132pd xmm1, xmm6, [0xdf7]
	vfnmadd132pd ymm6, ymm3, [0x9fd]
	vfnmadd132pd xmm4, xmm5, [0x87d]
	vfnmadd132pd ymm5, ymm4, [0xd07]
	vfnmadd132pd zmm7, zmm5, [0x615]
