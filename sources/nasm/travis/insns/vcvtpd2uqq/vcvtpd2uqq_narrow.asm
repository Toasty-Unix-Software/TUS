	vcvtpd2uqq xmm0, xmm6
	vcvtpd2uqq xmm5, oword [0xab4]
	vcvtpd2uqq ymm6, ymm7
	vcvtpd2uqq ymm6, ymm3
	vcvtpd2uqq zmm4, zmm5
	vcvtpd2uqq zmm7, zword [0x29d]

%ifdef ERROR
	vcvtpd2uqq xmm9, xmm12
	vcvtpd2uqq ymm12, ymm9
	vcvtpd2uqq zmm10, zmm15
	vcvtpd2uqq xmm16, xmm24
	vcvtpd2uqq ymm17, ymm17
	vcvtpd2uqq zmm30, zmm23
%endif
