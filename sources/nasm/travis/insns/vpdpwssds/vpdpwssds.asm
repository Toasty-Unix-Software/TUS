default rel
	vpdpwssds xmm1, xmm0, oword [0xc27]
	vpdpwssds xmm0, xmm3, xmm1
	vpdpwssds ymm4, ymm4, ymm2
	vpdpwssds ymm3, ymm7, yword [0x49b]
	vpdpwssds xmm5, xmm6, xmm1
	vpdpwssds xmm5, xmm1
	vpdpwssds xmm3, xmm0, xmm1
	vpdpwssds ymm6, ymm5, ymm2
	vpdpwssds ymm6, ymm2
	vpdpwssds ymm1, ymm2, ymm6
	vpdpwssds xmm8, xmm12, xmm10
	vpdpwssds ymm11, ymm11, ymm11
	vpdpwssds xmm12, xmm9, xmm10
	vpdpwssds ymm8, ymm11, ymm12
	vpdpwssds xmm31, xmm22, xmm30
	vpdpwssds ymm18, ymm23, ymm28
	vpdpwssds xmm25, xmm18, xmm26
	vpdpwssds ymm30, ymm30, ymm31
	vpdpwssds xmm1{k6}, xmm1, xmm7
	vpdpwssds ymm7{k2}, ymm3, ymm6
	vpdpwssds zmm5{k5}, zmm6, zword [0x62f]
	vpdpwssds xmm5{k6}{z}, xmm6, xmm1
	vpdpwssds ymm2{k5}{z}, ymm2, yword [0xdc2]
	vpdpwssds zmm0{k6}{z}, zmm1, zmm7
	vpdpwssds xmm6, xmm5, oword [eax+1]
	vpdpwssds xmm2, xmm1, oword [eax+64]
	vpdpwssds ymm7, ymm3, yword [eax+1]
	vpdpwssds ymm2, ymm2, yword [eax+64]
	vpdpwssds xmm2, xmm1, oword [eax+1]
	vpdpwssds xmm7, xmm4, oword [eax+64]
	vpdpwssds ymm5, ymm2, yword [eax+1]
	vpdpwssds ymm7, ymm7, yword [eax+64]
	vpdpwssds zmm1, zmm2, zword [eax+1]
	vpdpwssds zmm4, zmm3, zword [eax+64]
	vpdpwssds xmm7, xmm2, [0xfce]
	vpdpwssds ymm1, ymm1, [0x318]
	vpdpwssds xmm2, xmm1, [0x775]
	vpdpwssds ymm4, ymm7, [0xe6f]
	vpdpwssds zmm5, zmm3, [0xe60]
