	vcvttpd2udq xmm6, xmm3
	vcvttpd2udq xmm6, xmm2
	vcvttpd2udq xmm0, ymm5
	vcvttpd2udq xmm6, yword [0xd12]
	vcvttpd2udq ymm3, zword [0x5f5]
	vcvttpd2udq ymm0, zmm0

%ifdef ERROR
	vcvttpd2udq xmm8, xmm12
	vcvttpd2udq xmm11, ymm13
	vcvttpd2udq ymm9, zmm14
	vcvttpd2udq xmm19, xmm27
	vcvttpd2udq xmm26, ymm29
	vcvttpd2udq ymm19, zmm31
%endif
