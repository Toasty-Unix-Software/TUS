default rel
	vpshrdvw xmm2, xmm2, oword [0xe13]
	vpshrdvw xmm2, oword [0xe13]
	vpshrdvw xmm4, xmm5, oword [0xf35]
	vpshrdvw ymm1, ymm4, ymm3
	vpshrdvw ymm1, ymm3
	vpshrdvw ymm1, ymm4, ymm6
	vpshrdvw zmm2, zmm2, zmm4
	vpshrdvw zmm2, zmm4
	vpshrdvw zmm5, zmm0, zmm7
	vpshrdvw xmm11, xmm14, xmm15
	vpshrdvw ymm11, ymm15, ymm13
	vpshrdvw zmm9, zmm14, zmm15
	vpshrdvw xmm21, xmm16, xmm25
	vpshrdvw ymm30, ymm27, ymm23
	vpshrdvw zmm17, zmm17, zmm17
	vpshrdvw xmm2{k2}, xmm4, oword [0x7e3]
	vpshrdvw ymm5{k2}, ymm2, ymm2
	vpshrdvw zmm0{k3}, zmm0, zword [0xbdd]
	vpshrdvw xmm3{k3}{z}, xmm4, oword [0x11c]
	vpshrdvw ymm1{k7}{z}, ymm4, ymm0
	vpshrdvw zmm7{k7}{z}, zmm1, zmm5
	vpshrdvw xmm5, xmm3, oword [eax+1]
	vpshrdvw xmm6, xmm2, oword [eax+64]
	vpshrdvw ymm0, ymm6, yword [eax+1]
	vpshrdvw ymm6, ymm0, yword [eax+64]
	vpshrdvw zmm0, zmm6, zword [eax+1]
	vpshrdvw zmm6, zmm1, zword [eax+64]
	vpshrdvw xmm2, xmm0, [0xf99]
	vpshrdvw ymm5, ymm7, [0xf0c]
	vpshrdvw zmm1, zmm6, [0x1e7]
