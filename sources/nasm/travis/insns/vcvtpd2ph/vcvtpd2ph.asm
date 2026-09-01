default rel
	vcvtpd2ph xmm4, xmm0
	vcvtpd2ph xmm1, xmm0
	vcvtpd2ph xmm2, ymm2
	vcvtpd2ph xmm4, yword [0x9c0]
	vcvtpd2ph xmm3, zmm1
	vcvtpd2ph xmm5, zmm6
	vcvtpd2ph xmm9, xmm10
	vcvtpd2ph xmm11, ymm9
	vcvtpd2ph xmm13, zmm13
	vcvtpd2ph xmm25, xmm17
	vcvtpd2ph xmm18, ymm18
	vcvtpd2ph xmm19, zmm31
	vcvtpd2ph xmm4{k2}, xmm2
	vcvtpd2ph xmm0{k6}, ymm4
	vcvtpd2ph xmm6{k4}, zmm4
	vcvtpd2ph xmm1{k1}{z}, xmm0
	vcvtpd2ph xmm7{k5}{z}, yword [0xebf]
	vcvtpd2ph xmm7{k7}{z}, zmm2
	vcvtpd2ph xmm0, zmm3, {rd-sae}
	vcvtpd2ph xmm2, oword [eax+1]
	vcvtpd2ph xmm7, oword [eax+64]
	vcvtpd2ph xmm5, yword [eax+1]
	vcvtpd2ph xmm7, yword [eax+64]
	vcvtpd2ph xmm4, zword [eax+1]
	vcvtpd2ph xmm5, zword [eax+64]
	vcvtpd2ph xmm7, [0x335]
	vcvtpd2ph xmm7, [0x728]
	vcvtpd2ph xmm7, [0x852]
