	vcvttpd2uqq xmm1, xmm4
	vcvttpd2uqq xmm1, xmm1
	vcvttpd2uqq ymm6, yword [0xa40]
	vcvttpd2uqq ymm3, ymm7
	vcvttpd2uqq zmm5, zmm3
	vcvttpd2uqq zmm2, zword [0xf02]

%ifdef ERROR
	vcvttpd2uqq xmm13, xmm8
	vcvttpd2uqq ymm9, ymm12
	vcvttpd2uqq zmm11, zmm14
	vcvttpd2uqq xmm23, xmm30
	vcvttpd2uqq ymm26, ymm23
	vcvttpd2uqq zmm19, zmm23
%endif
