default rel
	vpdpwsuds xmm1, xmm2, oword [0x674]
	vpdpwsuds xmm3, xmm5, oword [0x834]
	vpdpwsuds ymm6, ymm7, ymm6
	vpdpwsuds ymm0, ymm7, ymm3
	vpdpwsuds xmm5, xmm0, xmm1
	vpdpwsuds xmm0, xmm2, xmm6
	vpdpwsuds ymm4, ymm2, ymm6
	vpdpwsuds ymm2, ymm6, ymm2
	vpdpwsuds xmm13, xmm14, xmm14
	vpdpwsuds ymm11, ymm11, ymm13
	vpdpwsuds xmm14, xmm10, xmm8
	vpdpwsuds ymm8, ymm15, ymm15
	vpdpwsuds xmm29, xmm24, xmm27
	vpdpwsuds ymm29, ymm27, ymm17
	vpdpwsuds xmm23, xmm18, xmm16
	vpdpwsuds ymm21, ymm24, ymm18
	vpdpwsuds xmm7{k1}, xmm0, xmm2
	vpdpwsuds ymm0{k6}, ymm2, ymm3
	vpdpwsuds zmm6{k1}, zmm3, zword [0x5ed]
	vpdpwsuds xmm2{k6}{z}, xmm6, xmm4
	vpdpwsuds ymm7{k7}{z}, ymm4, ymm7
	vpdpwsuds zmm7{k7}{z}, zmm2, zmm3
	vpdpwsuds xmm4, xmm6, oword [eax+1]
	vpdpwsuds xmm3, xmm2, oword [eax+64]
	vpdpwsuds ymm7, ymm1, yword [eax+1]
	vpdpwsuds ymm0, ymm7, yword [eax+64]
	vpdpwsuds xmm5, xmm6, oword [eax+1]
	vpdpwsuds xmm4, xmm6, oword [eax+64]
	vpdpwsuds ymm4, ymm5, yword [eax+1]
	vpdpwsuds ymm1, ymm2, yword [eax+64]
	vpdpwsuds zmm6, zmm0, zword [eax+1]
	vpdpwsuds zmm0, zmm6, zword [eax+64]
	vpdpwsuds xmm2, xmm5, [0x136]
	vpdpwsuds ymm2, ymm5, [0x918]
	vpdpwsuds xmm6, xmm2, [0x4ab]
	vpdpwsuds ymm3, ymm1, [0x313]
	vpdpwsuds zmm0, zmm0, [0xb02]
