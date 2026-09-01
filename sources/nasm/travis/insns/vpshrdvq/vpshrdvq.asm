default rel
	vpshrdvq xmm0, xmm7, xmm4
	vpshrdvq xmm0, xmm4
	vpshrdvq xmm2, xmm1, xmm1
	vpshrdvq ymm1, ymm4, ymm0
	vpshrdvq ymm1, ymm0
	vpshrdvq ymm7, ymm6, ymm0
	vpshrdvq zmm4, zmm3, zmm5
	vpshrdvq zmm4, zmm5
	vpshrdvq zmm5, zmm3, zword [0xe49]
	vpshrdvq xmm10, xmm10, xmm13
	vpshrdvq ymm15, ymm13, ymm13
	vpshrdvq zmm13, zmm13, zmm11
	vpshrdvq xmm25, xmm28, xmm26
	vpshrdvq ymm31, ymm25, ymm17
	vpshrdvq zmm25, zmm19, zmm22
	vpshrdvq xmm5{k5}, xmm4, oword [0x30e]
	vpshrdvq ymm6{k5}, ymm5, ymm6
	vpshrdvq zmm0{k3}, zmm6, zmm1
	vpshrdvq xmm4{k6}{z}, xmm4, xmm3
	vpshrdvq ymm1{k3}{z}, ymm4, ymm7
	vpshrdvq zmm6{k6}{z}, zmm7, zmm1
	vpshrdvq xmm3, xmm5, oword [eax+1]
	vpshrdvq xmm1, xmm7, oword [eax+64]
	vpshrdvq ymm0, ymm5, yword [eax+1]
	vpshrdvq ymm3, ymm1, yword [eax+64]
	vpshrdvq zmm2, zmm3, zword [eax+1]
	vpshrdvq zmm7, zmm3, zword [eax+64]
	vpshrdvq xmm7, xmm6, [0x5d8]
	vpshrdvq ymm0, ymm4, [0xbce]
	vpshrdvq zmm4, zmm3, [0x3c7]
