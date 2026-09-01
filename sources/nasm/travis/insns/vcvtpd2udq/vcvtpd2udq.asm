default rel
	vcvtpd2udq xmm7, xmm3
	vcvtpd2udq xmm1, xmm6
	vcvtpd2udq xmm4, ymm2
	vcvtpd2udq xmm1, ymm4
	vcvtpd2udq ymm1, zmm3
	vcvtpd2udq ymm7, zmm5
	vcvtpd2udq xmm15, xmm13
	vcvtpd2udq xmm8, ymm11
	vcvtpd2udq ymm8, zmm13
	vcvtpd2udq xmm28, xmm23
	vcvtpd2udq xmm31, ymm24
	vcvtpd2udq ymm28, zmm27
	vcvtpd2udq xmm2{k2}, oword [0x195]
	vcvtpd2udq xmm1{k7}, yword [0x515]
	vcvtpd2udq ymm6{k5}, zword [0x4da]
	vcvtpd2udq xmm1{k2}{z}, xmm5
	vcvtpd2udq xmm3{k5}{z}, ymm4
	vcvtpd2udq ymm1{k6}{z}, zmm7
	vcvtpd2udq ymm1, zmm3, {rd-sae}
	vcvtpd2udq xmm0, oword [eax+1]
	vcvtpd2udq xmm6, oword [eax+64]
	vcvtpd2udq xmm5, yword [eax+1]
	vcvtpd2udq xmm2, yword [eax+64]
	vcvtpd2udq ymm2, zword [eax+1]
	vcvtpd2udq ymm4, zword [eax+64]
	vcvtpd2udq xmm2, [0x4a6]
	vcvtpd2udq xmm4, [0xc79]
	vcvtpd2udq ymm5, [0xdfa]
