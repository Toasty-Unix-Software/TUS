default rel
	vfmadd231ps xmm5, xmm7, xmm0
	vfmadd231ps xmm2, xmm4, xmm0
	vfmadd231ps ymm0, ymm6, yword [0x5aa]
	vfmadd231ps ymm2, ymm5, ymm5
	vfmadd231ps xmm2, xmm5, xmm4
	vfmadd231ps xmm7, xmm2, xmm2
	vfmadd231ps ymm0, ymm0, ymm3
	vfmadd231ps ymm6, ymm0, yword [0x14d]
	vfmadd231ps xmm10, xmm11, xmm13
	vfmadd231ps ymm13, ymm14, ymm15
	vfmadd231ps xmm9, xmm9, xmm14
	vfmadd231ps ymm8, ymm9, ymm12
	vfmadd231ps xmm26, xmm31, xmm31
	vfmadd231ps ymm29, ymm20, ymm16
	vfmadd231ps xmm20, xmm16, xmm31
	vfmadd231ps ymm31, ymm20, ymm17
	vfmadd231ps xmm7{k5}, xmm5, xmm0
	vfmadd231ps ymm5{k4}, ymm6, yword [0xf3d]
	vfmadd231ps zmm1{k2}, zmm2, zword [0x12f]
	vfmadd231ps xmm7{k4}{z}, xmm7, xmm6
	vfmadd231ps ymm3{k4}{z}, ymm7, yword [0x4bc]
	vfmadd231ps zmm5{k5}{z}, zmm2, zmm5
	vfmadd231ps zmm5, zmm5, zmm3, {rd-sae}
	vfmadd231ps xmm7, xmm6, oword [eax+1]
	vfmadd231ps xmm4, xmm3, oword [eax+64]
	vfmadd231ps ymm3, ymm1, yword [eax+1]
	vfmadd231ps ymm7, ymm7, yword [eax+64]
	vfmadd231ps xmm5, xmm2, oword [eax+1]
	vfmadd231ps xmm7, xmm0, oword [eax+64]
	vfmadd231ps ymm7, ymm1, yword [eax+1]
	vfmadd231ps ymm4, ymm2, yword [eax+64]
	vfmadd231ps zmm0, zmm3, zword [eax+1]
	vfmadd231ps zmm4, zmm1, zword [eax+64]
	vfmadd231ps xmm2, xmm0, [0x230]
	vfmadd231ps ymm6, ymm3, [0x234]
	vfmadd231ps xmm0, xmm6, [0x707]
	vfmadd231ps ymm7, ymm5, [0xffe]
	vfmadd231ps zmm0, zmm3, [0xeba]
