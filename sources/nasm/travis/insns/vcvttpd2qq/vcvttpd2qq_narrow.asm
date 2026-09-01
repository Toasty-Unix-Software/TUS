	vcvttpd2qq xmm7, xmm1
	vcvttpd2qq xmm6, oword [0xdf5]
	vcvttpd2qq ymm5, yword [0xb7b]
	vcvttpd2qq ymm3, yword [0xbfe]
	vcvttpd2qq zmm0, zword [0x1cf]
	vcvttpd2qq zmm1, zmm7

%ifdef ERROR
	vcvttpd2qq xmm8, xmm8
	vcvttpd2qq ymm15, ymm11
	vcvttpd2qq zmm14, zmm15
	vcvttpd2qq xmm19, xmm26
	vcvttpd2qq ymm25, ymm17
	vcvttpd2qq zmm22, zmm21
%endif
