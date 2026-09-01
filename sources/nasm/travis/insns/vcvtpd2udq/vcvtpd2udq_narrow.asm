	vcvtpd2udq xmm7, xmm3
	vcvtpd2udq xmm1, xmm6
	vcvtpd2udq xmm4, ymm2
	vcvtpd2udq xmm1, ymm4
	vcvtpd2udq ymm1, zmm3
	vcvtpd2udq ymm7, zmm5

%ifdef ERROR
	vcvtpd2udq xmm15, xmm13
	vcvtpd2udq xmm8, ymm11
	vcvtpd2udq ymm8, zmm13
	vcvtpd2udq xmm28, xmm23
	vcvtpd2udq xmm31, ymm24
	vcvtpd2udq ymm28, zmm27
%endif
