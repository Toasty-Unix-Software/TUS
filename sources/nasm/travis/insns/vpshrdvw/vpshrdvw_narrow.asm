	vpshrdvw xmm2, xmm2, oword [0xe13]
	vpshrdvw xmm2, oword [0xe13]
	vpshrdvw xmm4, xmm5, oword [0xf35]
	vpshrdvw ymm1, ymm4, ymm3
	vpshrdvw ymm1, ymm3
	vpshrdvw ymm1, ymm4, ymm6
	vpshrdvw zmm2, zmm2, zmm4
	vpshrdvw zmm2, zmm4
	vpshrdvw zmm5, zmm0, zmm7

%ifdef ERROR
	vpshrdvw xmm11, xmm14, xmm15
	vpshrdvw ymm11, ymm15, ymm13
	vpshrdvw zmm9, zmm14, zmm15
	vpshrdvw xmm21, xmm16, xmm25
	vpshrdvw ymm30, ymm27, ymm23
	vpshrdvw zmm17, zmm17, zmm17
%endif
