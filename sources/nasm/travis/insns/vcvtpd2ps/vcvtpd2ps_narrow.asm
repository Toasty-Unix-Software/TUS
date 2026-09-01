	vcvtpd2ps xmm5, xmm6
	vcvtpd2ps xmm5, xmm6
	vcvtpd2ps xmm1, oword [0x17d]
	vcvtpd2ps xmm3, oword [0x565]
	vcvtpd2ps xmm2, ymm5
	vcvtpd2ps xmm5, ymm5
	vcvtpd2ps xmm1, yword [0x522]
	vcvtpd2ps xmm0, yword [0x37c]

%ifdef ERROR
	vcvtpd2ps xmm12, xmm14
	vcvtpd2ps xmm10, oword [0x443]
	vcvtpd2ps xmm15, ymm15
	vcvtpd2ps xmm13, yword [0x8a5]
	vcvtpd2ps xmm26, xmm24
	vcvtpd2ps xmm25, oword [0xb2f]
	vcvtpd2ps xmm31, ymm30
	vcvtpd2ps xmm26, yword [0xbc2]
%endif
