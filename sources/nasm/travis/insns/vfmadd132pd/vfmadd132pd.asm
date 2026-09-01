default rel
	vfmadd132pd xmm2, xmm4, xmm3
	vfmadd132pd xmm0, xmm1, xmm7
	vfmadd132pd ymm6, ymm7, yword [0x911]
	vfmadd132pd ymm4, ymm1, ymm3
	vfmadd132pd xmm7, xmm0, oword [0xfb6]
	vfmadd132pd xmm5, xmm4, oword [0x1fa]
	vfmadd132pd ymm2, ymm5, ymm7
	vfmadd132pd ymm6, ymm2, ymm7
	vfmadd132pd xmm15, xmm12, xmm13
	vfmadd132pd ymm8, ymm13, ymm14
	vfmadd132pd xmm10, xmm9, xmm8
	vfmadd132pd ymm11, ymm13, ymm10
	vfmadd132pd xmm30, xmm22, xmm19
	vfmadd132pd ymm20, ymm23, ymm23
	vfmadd132pd xmm23, xmm20, xmm29
	vfmadd132pd ymm23, ymm20, ymm16
	vfmadd132pd xmm0{k3}, xmm5, oword [0xc6a]
	vfmadd132pd ymm2{k3}, ymm4, yword [0x563]
	vfmadd132pd zmm4{k2}, zmm2, zmm1
	vfmadd132pd xmm0{k4}{z}, xmm5, xmm1
	vfmadd132pd ymm0{k4}{z}, ymm7, yword [0x92b]
	vfmadd132pd zmm4{k2}{z}, zmm7, zmm5
	vfmadd132pd zmm5, zmm4, zmm2, {rz-sae}
	vfmadd132pd xmm2, xmm2, oword [eax+1]
	vfmadd132pd xmm2, xmm4, oword [eax+64]
	vfmadd132pd ymm3, ymm3, yword [eax+1]
	vfmadd132pd ymm7, ymm2, yword [eax+64]
	vfmadd132pd xmm0, xmm2, oword [eax+1]
	vfmadd132pd xmm4, xmm3, oword [eax+64]
	vfmadd132pd ymm3, ymm7, yword [eax+1]
	vfmadd132pd ymm4, ymm0, yword [eax+64]
	vfmadd132pd zmm0, zmm1, zword [eax+1]
	vfmadd132pd zmm0, zmm4, zword [eax+64]
	vfmadd132pd xmm4, xmm3, [0x802]
	vfmadd132pd ymm4, ymm1, [0x8ed]
	vfmadd132pd xmm1, xmm0, [0x35d]
	vfmadd132pd ymm2, ymm2, [0xe38]
	vfmadd132pd zmm1, zmm4, [0x9aa]
