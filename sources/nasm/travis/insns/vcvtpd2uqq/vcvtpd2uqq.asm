default rel
	vcvtpd2uqq xmm0, xmm6
	vcvtpd2uqq xmm5, oword [0xab4]
	vcvtpd2uqq ymm6, ymm7
	vcvtpd2uqq ymm6, ymm3
	vcvtpd2uqq zmm4, zmm5
	vcvtpd2uqq zmm7, zword [0x29d]
	vcvtpd2uqq xmm9, xmm12
	vcvtpd2uqq ymm12, ymm9
	vcvtpd2uqq zmm10, zmm15
	vcvtpd2uqq xmm16, xmm24
	vcvtpd2uqq ymm17, ymm17
	vcvtpd2uqq zmm30, zmm23
	vcvtpd2uqq xmm3{k3}, xmm2
	vcvtpd2uqq ymm1{k2}, ymm5
	vcvtpd2uqq zmm2{k3}, zmm6
	vcvtpd2uqq xmm4{k1}{z}, xmm2
	vcvtpd2uqq ymm3{k5}{z}, ymm7
	vcvtpd2uqq zmm1{k5}{z}, zmm4
	vcvtpd2uqq zmm7, zmm2, {ru-sae}
	vcvtpd2uqq xmm6, oword [eax+1]
	vcvtpd2uqq xmm1, oword [eax+64]
	vcvtpd2uqq ymm4, yword [eax+1]
	vcvtpd2uqq ymm3, yword [eax+64]
	vcvtpd2uqq zmm2, zword [eax+1]
	vcvtpd2uqq zmm5, zword [eax+64]
	vcvtpd2uqq xmm7, [0xaa3]
	vcvtpd2uqq ymm2, [0x37e]
	vcvtpd2uqq zmm1, [0xaaa]
