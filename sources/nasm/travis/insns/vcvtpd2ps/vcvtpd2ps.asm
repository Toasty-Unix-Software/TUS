default rel
	vcvtpd2ps xmm5, xmm6
	vcvtpd2ps xmm5, xmm6
	vcvtpd2ps xmm1, oword [0x17d]
	vcvtpd2ps xmm3, oword [0x565]
	vcvtpd2ps xmm2, ymm5
	vcvtpd2ps xmm5, ymm5
	vcvtpd2ps xmm1, yword [0x522]
	vcvtpd2ps xmm0, yword [0x37c]
	vcvtpd2ps xmm12, xmm14
	vcvtpd2ps xmm10, oword [0x443]
	vcvtpd2ps xmm15, ymm15
	vcvtpd2ps xmm13, yword [0x8a5]
	vcvtpd2ps xmm26, xmm24
	vcvtpd2ps xmm25, oword [0xb2f]
	vcvtpd2ps xmm31, ymm30
	vcvtpd2ps xmm26, yword [0xbc2]
	vcvtpd2ps xmm0{k4}, xmm0
	vcvtpd2ps xmm7{k2}, ymm4
	vcvtpd2ps ymm6{k7}, zmm4
	vcvtpd2ps xmm7{k6}{z}, xmm5
	vcvtpd2ps xmm4{k3}{z}, ymm2
	vcvtpd2ps ymm4{k5}{z}, zmm7
	vcvtpd2ps ymm7, zmm7, {rd-sae}
	vcvtpd2ps xmm1, oword [eax+1]
	vcvtpd2ps xmm5, oword [eax+64]
	vcvtpd2ps xmm5, yword [eax+1]
	vcvtpd2ps xmm1, yword [eax+64]
	vcvtpd2ps xmm2, oword [eax+1]
	vcvtpd2ps xmm0, oword [eax+64]
	vcvtpd2ps xmm1, yword [eax+1]
	vcvtpd2ps xmm7, yword [eax+64]
	vcvtpd2ps ymm1, zword [eax+1]
	vcvtpd2ps ymm5, zword [eax+64]
	vcvtpd2ps xmm7, [0x927]
	vcvtpd2ps xmm5, [0xc3f]
	vcvtpd2ps xmm0, [0x529]
	vcvtpd2ps xmm4, [0xc67]
	vcvtpd2ps ymm5, [0x880]
