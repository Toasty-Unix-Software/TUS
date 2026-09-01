default rel
	vcvtpd2qq xmm0, xmm1
	vcvtpd2qq xmm5, oword [0xdd8]
	vcvtpd2qq ymm3, yword [0x4e3]
	vcvtpd2qq ymm6, ymm4
	vcvtpd2qq zmm2, zword [0x7d8]
	vcvtpd2qq zmm6, zmm5
	vcvtpd2qq xmm11, xmm8
	vcvtpd2qq ymm14, ymm9
	vcvtpd2qq zmm14, zmm13
	vcvtpd2qq xmm24, xmm19
	vcvtpd2qq ymm26, ymm23
	vcvtpd2qq zmm19, zmm26
	vcvtpd2qq xmm5{k5}, oword [0x363]
	vcvtpd2qq ymm6{k7}, ymm6
	vcvtpd2qq zmm3{k1}, zmm2
	vcvtpd2qq xmm1{k7}{z}, xmm0
	vcvtpd2qq ymm3{k3}{z}, yword [0xf33]
	vcvtpd2qq zmm7{k7}{z}, zmm7
	vcvtpd2qq zmm0, zmm3, {ru-sae}
	vcvtpd2qq xmm7, oword [eax+1]
	vcvtpd2qq xmm0, oword [eax+64]
	vcvtpd2qq ymm5, yword [eax+1]
	vcvtpd2qq ymm2, yword [eax+64]
	vcvtpd2qq zmm6, zword [eax+1]
	vcvtpd2qq zmm1, zword [eax+64]
	vcvtpd2qq xmm6, [0x251]
	vcvtpd2qq ymm0, [0x53c]
	vcvtpd2qq zmm3, [0x8c7]
