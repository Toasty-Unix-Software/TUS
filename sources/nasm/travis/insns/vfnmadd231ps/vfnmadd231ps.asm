default rel
	vfnmadd231ps xmm3, xmm1, xmm7
	vfnmadd231ps xmm0, xmm7, xmm6
	vfnmadd231ps ymm5, ymm6, ymm4
	vfnmadd231ps ymm6, ymm3, yword [0xac9]
	vfnmadd231ps xmm7, xmm7, xmm3
	vfnmadd231ps xmm5, xmm1, xmm3
	vfnmadd231ps ymm5, ymm7, ymm5
	vfnmadd231ps ymm5, ymm3, yword [0xdfe]
	vfnmadd231ps xmm12, xmm11, xmm14
	vfnmadd231ps ymm10, ymm15, ymm14
	vfnmadd231ps xmm13, xmm11, xmm8
	vfnmadd231ps ymm11, ymm13, ymm11
	vfnmadd231ps xmm26, xmm24, xmm24
	vfnmadd231ps ymm16, ymm22, ymm30
	vfnmadd231ps xmm21, xmm29, xmm28
	vfnmadd231ps ymm27, ymm18, ymm24
	vfnmadd231ps xmm3{k1}, xmm3, oword [0x726]
	vfnmadd231ps ymm1{k6}, ymm7, ymm0
	vfnmadd231ps zmm2{k6}, zmm5, zmm3
	vfnmadd231ps xmm5{k5}{z}, xmm3, xmm0
	vfnmadd231ps ymm2{k2}{z}, ymm6, ymm3
	vfnmadd231ps zmm0{k7}{z}, zmm7, zword [0xdda]
	vfnmadd231ps zmm7, zmm6, zmm2, {rz-sae}
	vfnmadd231ps xmm3, xmm1, oword [eax+1]
	vfnmadd231ps xmm3, xmm1, oword [eax+64]
	vfnmadd231ps ymm2, ymm4, yword [eax+1]
	vfnmadd231ps ymm3, ymm0, yword [eax+64]
	vfnmadd231ps xmm5, xmm2, oword [eax+1]
	vfnmadd231ps xmm0, xmm3, oword [eax+64]
	vfnmadd231ps ymm0, ymm0, yword [eax+1]
	vfnmadd231ps ymm7, ymm5, yword [eax+64]
	vfnmadd231ps zmm3, zmm1, zword [eax+1]
	vfnmadd231ps zmm0, zmm4, zword [eax+64]
	vfnmadd231ps xmm5, xmm1, [0xa7d]
	vfnmadd231ps ymm0, ymm7, [0xe27]
	vfnmadd231ps xmm5, xmm3, [0x7cb]
	vfnmadd231ps ymm7, ymm3, [0x592]
	vfnmadd231ps zmm1, zmm2, [0x628]
