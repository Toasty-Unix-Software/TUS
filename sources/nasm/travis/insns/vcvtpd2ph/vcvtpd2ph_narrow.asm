	vcvtpd2ph xmm4, xmm0
	vcvtpd2ph xmm1, xmm0
	vcvtpd2ph xmm2, ymm2
	vcvtpd2ph xmm4, yword [0x9c0]
	vcvtpd2ph xmm3, zmm1
	vcvtpd2ph xmm5, zmm6

%ifdef ERROR
	vcvtpd2ph xmm9, xmm10
	vcvtpd2ph xmm11, ymm9
	vcvtpd2ph xmm13, zmm13
	vcvtpd2ph xmm25, xmm17
	vcvtpd2ph xmm18, ymm18
	vcvtpd2ph xmm19, zmm31
%endif
