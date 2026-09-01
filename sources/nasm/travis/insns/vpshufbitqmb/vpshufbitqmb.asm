default rel
	vpshufbitqmb k7, xmm2, oword [0x57f]
	vpshufbitqmb k5, xmm2, xmm1
	vpshufbitqmb k2, ymm1, ymm1
	vpshufbitqmb k6, ymm5, ymm5
	vpshufbitqmb k3, zmm6, zword [0xbbf]
	vpshufbitqmb k5, zmm7, zmm5
	vpshufbitqmb k5, xmm13, xmm9
	vpshufbitqmb k3, ymm8, ymm13
	vpshufbitqmb k2, zmm11, zmm10
	vpshufbitqmb k3, xmm23, xmm30
	vpshufbitqmb k7, ymm21, ymm19
	vpshufbitqmb k1, zmm25, zmm20
	vpshufbitqmb k1{k6}, xmm7, oword [0xafa]
	vpshufbitqmb k3{k5}, ymm3, ymm7
	vpshufbitqmb k6{k6}, zmm4, zmm4
	vpshufbitqmb k5, xmm5, oword [eax+1]
	vpshufbitqmb k1, xmm7, oword [eax+64]
	vpshufbitqmb k5, ymm2, yword [eax+1]
	vpshufbitqmb k7, ymm0, yword [eax+64]
	vpshufbitqmb k1, zmm3, zword [eax+1]
	vpshufbitqmb k6, zmm1, zword [eax+64]
	vpshufbitqmb k4, xmm4, [0x87f]
	vpshufbitqmb k5, ymm3, [0x7d5]
	vpshufbitqmb k4, zmm3, [0xbee]
