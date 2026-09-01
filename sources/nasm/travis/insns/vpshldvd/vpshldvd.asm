default rel
	vpshldvd xmm2, xmm7, oword [0xf31]
	vpshldvd xmm2, oword [0xf31]
	vpshldvd xmm4, xmm4, oword [0x2e7]
	vpshldvd ymm7, ymm2, yword [0xe58]
	vpshldvd ymm7, yword [0xe58]
	vpshldvd ymm7, ymm0, yword [0xf81]
	vpshldvd zmm0, zmm2, zmm7
	vpshldvd zmm0, zmm7
	vpshldvd zmm6, zmm7, zword [0xd04]
	vpshldvd xmm15, xmm11, xmm15
	vpshldvd ymm15, ymm8, ymm13
	vpshldvd zmm9, zmm8, zmm8
	vpshldvd xmm26, xmm17, xmm26
	vpshldvd ymm18, ymm16, ymm26
	vpshldvd zmm22, zmm28, zmm20
	vpshldvd xmm2{k4}, xmm3, xmm0
	vpshldvd ymm7{k3}, ymm2, yword [0xdcb]
	vpshldvd zmm7{k1}, zmm4, zmm7
	vpshldvd xmm6{k4}{z}, xmm0, xmm0
	vpshldvd ymm1{k1}{z}, ymm6, yword [0x6ce]
	vpshldvd zmm4{k7}{z}, zmm1, zword [0xa7f]
	vpshldvd xmm6, xmm1, oword [eax+1]
	vpshldvd xmm0, xmm0, oword [eax+64]
	vpshldvd ymm6, ymm1, yword [eax+1]
	vpshldvd ymm4, ymm5, yword [eax+64]
	vpshldvd zmm1, zmm1, zword [eax+1]
	vpshldvd zmm1, zmm2, zword [eax+64]
	vpshldvd xmm1, xmm7, [0x4e2]
	vpshldvd ymm4, ymm3, [0x916]
	vpshldvd zmm1, zmm4, [0xd30]
