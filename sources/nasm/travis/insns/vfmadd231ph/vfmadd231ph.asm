default rel
	vfmadd231ph xmm6, xmm7, oword [0xc7c]
	vfmadd231ph xmm6, oword [0xc7c]
	vfmadd231ph xmm2, xmm5, xmm0
	vfmadd231ph ymm6, ymm1, ymm5
	vfmadd231ph ymm6, ymm5
	vfmadd231ph ymm3, ymm7, ymm6
	vfmadd231ph zmm6, zmm3, zmm6
	vfmadd231ph zmm6, zmm6
	vfmadd231ph zmm7, zmm2, zmm3
	vfmadd231ph xmm14, xmm10, xmm10
	vfmadd231ph ymm14, ymm10, ymm8
	vfmadd231ph zmm14, zmm11, zmm11
	vfmadd231ph xmm28, xmm17, xmm16
	vfmadd231ph ymm24, ymm20, ymm20
	vfmadd231ph zmm30, zmm16, zmm21
	vfmadd231ph xmm1{k4}, xmm7, xmm6
	vfmadd231ph ymm4{k5}, ymm2, yword [0x533]
	vfmadd231ph zmm2{k6}, zmm5, zmm3
	vfmadd231ph xmm7{k6}{z}, xmm3, oword [0xd15]
	vfmadd231ph ymm7{k1}{z}, ymm6, ymm7
	vfmadd231ph zmm0{k2}{z}, zmm3, zword [0x1ea]
	vfmadd231ph zmm5, zmm3, zmm1, {rd-sae}
	vfmadd231ph xmm0, xmm1, oword [eax+1]
	vfmadd231ph xmm4, xmm0, oword [eax+64]
	vfmadd231ph ymm2, ymm0, yword [eax+1]
	vfmadd231ph ymm1, ymm2, yword [eax+64]
	vfmadd231ph zmm3, zmm4, zword [eax+1]
	vfmadd231ph zmm4, zmm5, zword [eax+64]
	vfmadd231ph xmm1, xmm3, [0x691]
	vfmadd231ph ymm5, ymm4, [0xc43]
	vfmadd231ph zmm6, zmm5, [0x3d2]
