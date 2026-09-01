default rel
	vpshrdvd xmm7, xmm6, xmm7
	vpshrdvd xmm7, xmm7
	vpshrdvd xmm2, xmm4, xmm3
	vpshrdvd ymm6, ymm1, yword [0xb98]
	vpshrdvd ymm6, yword [0xb98]
	vpshrdvd ymm3, ymm5, yword [0x7c4]
	vpshrdvd zmm6, zmm4, zmm1
	vpshrdvd zmm6, zmm1
	vpshrdvd zmm3, zmm6, zword [0xba7]
	vpshrdvd xmm11, xmm8, xmm11
	vpshrdvd ymm9, ymm12, ymm10
	vpshrdvd zmm15, zmm10, zmm15
	vpshrdvd xmm30, xmm21, xmm25
	vpshrdvd ymm21, ymm20, ymm28
	vpshrdvd zmm25, zmm31, zmm16
	vpshrdvd xmm4{k7}, xmm1, xmm7
	vpshrdvd ymm3{k4}, ymm6, ymm0
	vpshrdvd zmm7{k5}, zmm5, zword [0x6ce]
	vpshrdvd xmm6{k4}{z}, xmm4, xmm7
	vpshrdvd ymm7{k4}{z}, ymm5, ymm0
	vpshrdvd zmm4{k4}{z}, zmm1, zword [0x2f2]
	vpshrdvd xmm1, xmm4, oword [eax+1]
	vpshrdvd xmm4, xmm2, oword [eax+64]
	vpshrdvd ymm6, ymm4, yword [eax+1]
	vpshrdvd ymm6, ymm6, yword [eax+64]
	vpshrdvd zmm7, zmm5, zword [eax+1]
	vpshrdvd zmm6, zmm0, zword [eax+64]
	vpshrdvd xmm5, xmm7, [0x7e6]
	vpshrdvd ymm5, ymm3, [0x233]
	vpshrdvd zmm3, zmm2, [0x2b7]
