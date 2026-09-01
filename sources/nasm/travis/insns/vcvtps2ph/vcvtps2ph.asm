default rel
	vcvtps2ph xmm4, ymm5, 0xe0
	vcvtps2ph xmm6, ymm7, 0xe8
	vcvtps2ph xmm5, xmm6, 0x22
	vcvtps2ph xmm3, xmm7, 0x2c
	vcvtps2ph xmm1, xmm7, 0x5d
	vcvtps2ph xmm4, xmm5, 0xab
	vcvtps2ph xmm0, ymm4, 0xb1
	vcvtps2ph xmm3, ymm0, 0xf0
	vcvtps2ph xmm11, ymm9, 0x5e
	vcvtps2ph xmm14, xmm11, 0x68
	vcvtps2ph xmm12, xmm14, 0x75
	vcvtps2ph xmm14, ymm15, 0xa9
	vcvtps2ph xmm18, ymm26, 0x43
	vcvtps2ph xmm25, xmm27, 0xe5
	vcvtps2ph xmm31, xmm16, 0xc4
	vcvtps2ph xmm18, ymm16, 0x12
	vcvtps2ph xmm5{k7}, xmm5, 0x5c
	vcvtps2ph xmm7{k5}, ymm3, 0x45
	vcvtps2ph ymm7{k4}, zmm3, 0x7d
	vcvtps2ph qword [0xe5a]{k6}, xmm7, 0x5c
	vcvtps2ph oword [0xa5c]{k5}, ymm7, 0xa2
	vcvtps2ph yword [0xc45]{k7}, zmm4, 0xf5
	vcvtps2ph oword [eax+1], ymm0, 0xd3
	vcvtps2ph oword [eax+64], ymm3, 0x62
	vcvtps2ph qword [eax+1], xmm0, 0x5e
	vcvtps2ph qword [eax+64], xmm7, 0x25
	vcvtps2ph qword [eax+1], xmm5, 0xff
	vcvtps2ph qword [eax+64], xmm5, 0xf8
	vcvtps2ph oword [eax+1], ymm2, 0x21
	vcvtps2ph oword [eax+64], ymm5, 0xb2
	vcvtps2ph yword [eax+1], zmm1, 0xae
	vcvtps2ph yword [eax+64], zmm6, 0xe4
	vcvtps2ph [0x23e], ymm6, 0x34
	vcvtps2ph [0xd75], xmm7, 0x7f
	vcvtps2ph [0x72a], xmm5, 0x19
	vcvtps2ph [0xd40], ymm5, 0xd2
	vcvtps2ph [0x8f5], zmm3, 0xf9
