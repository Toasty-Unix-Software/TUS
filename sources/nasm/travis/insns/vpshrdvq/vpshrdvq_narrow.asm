	vpshrdvq xmm0, xmm7, xmm4
	vpshrdvq xmm0, xmm4
	vpshrdvq xmm2, xmm1, xmm1
	vpshrdvq ymm1, ymm4, ymm0
	vpshrdvq ymm1, ymm0
	vpshrdvq ymm7, ymm6, ymm0
	vpshrdvq zmm4, zmm3, zmm5
	vpshrdvq zmm4, zmm5
	vpshrdvq zmm5, zmm3, zword [0xe49]

%ifdef ERROR
	vpshrdvq xmm10, xmm10, xmm13
	vpshrdvq ymm15, ymm13, ymm13
	vpshrdvq zmm13, zmm13, zmm11
	vpshrdvq xmm25, xmm28, xmm26
	vpshrdvq ymm31, ymm25, ymm17
	vpshrdvq zmm25, zmm19, zmm22
%endif
