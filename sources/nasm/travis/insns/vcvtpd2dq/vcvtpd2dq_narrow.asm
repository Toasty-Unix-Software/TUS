	vcvtpd2dq xmm3, xmm5
	vcvtpd2dq xmm4, xmm5
	vcvtpd2dq xmm0, oword [0x413]
	vcvtpd2dq xmm6, oword [0x78b]
	vcvtpd2dq xmm7, ymm1
	vcvtpd2dq xmm3, ymm2
	vcvtpd2dq xmm5, yword [0x5e6]
	vcvtpd2dq xmm0, yword [0xfb3]

%ifdef ERROR
	vcvtpd2dq xmm15, xmm14
	vcvtpd2dq xmm9, oword [0xa0a]
	vcvtpd2dq xmm15, ymm12
	vcvtpd2dq xmm15, yword [0x3f6]
	vcvtpd2dq xmm16, xmm28
	vcvtpd2dq xmm29, oword [0xb06]
	vcvtpd2dq xmm26, ymm27
	vcvtpd2dq xmm19, yword [0x5e2]
%endif
