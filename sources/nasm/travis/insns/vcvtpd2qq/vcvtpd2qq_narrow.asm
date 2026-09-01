	vcvtpd2qq xmm0, xmm1
	vcvtpd2qq xmm5, oword [0xdd8]
	vcvtpd2qq ymm3, yword [0x4e3]
	vcvtpd2qq ymm6, ymm4
	vcvtpd2qq zmm2, zword [0x7d8]
	vcvtpd2qq zmm6, zmm5

%ifdef ERROR
	vcvtpd2qq xmm11, xmm8
	vcvtpd2qq ymm14, ymm9
	vcvtpd2qq zmm14, zmm13
	vcvtpd2qq xmm24, xmm19
	vcvtpd2qq ymm26, ymm23
	vcvtpd2qq zmm19, zmm26
%endif
