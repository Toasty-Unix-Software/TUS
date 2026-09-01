	vpshrdvd xmm7, xmm6, xmm7
	vpshrdvd xmm7, xmm7
	vpshrdvd xmm2, xmm4, xmm3
	vpshrdvd ymm6, ymm1, yword [0xb98]
	vpshrdvd ymm6, yword [0xb98]
	vpshrdvd ymm3, ymm5, yword [0x7c4]
	vpshrdvd zmm6, zmm4, zmm1
	vpshrdvd zmm6, zmm1
	vpshrdvd zmm3, zmm6, zword [0xba7]

%ifdef ERROR
	vpshrdvd xmm11, xmm8, xmm11
	vpshrdvd ymm9, ymm12, ymm10
	vpshrdvd zmm15, zmm10, zmm15
	vpshrdvd xmm30, xmm21, xmm25
	vpshrdvd ymm21, ymm20, ymm28
	vpshrdvd zmm25, zmm31, zmm16
%endif
