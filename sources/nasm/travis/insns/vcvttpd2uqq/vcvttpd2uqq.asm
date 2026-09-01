default rel
	vcvttpd2uqq xmm1, xmm4
	vcvttpd2uqq xmm1, xmm1
	vcvttpd2uqq ymm6, yword [0xa40]
	vcvttpd2uqq ymm3, ymm7
	vcvttpd2uqq zmm5, zmm3
	vcvttpd2uqq zmm2, zword [0xf02]
	vcvttpd2uqq xmm13, xmm8
	vcvttpd2uqq ymm9, ymm12
	vcvttpd2uqq zmm11, zmm14
	vcvttpd2uqq xmm23, xmm30
	vcvttpd2uqq ymm26, ymm23
	vcvttpd2uqq zmm19, zmm23
	vcvttpd2uqq xmm7{k7}, xmm0
	vcvttpd2uqq ymm7{k6}, ymm2
	vcvttpd2uqq zmm1{k3}, zmm6
	vcvttpd2uqq xmm0{k4}{z}, xmm6
	vcvttpd2uqq ymm2{k1}{z}, yword [0xf1a]
	vcvttpd2uqq zmm0{k1}{z}, zmm0
	vcvttpd2uqq zmm0, zmm5, {sae}
	vcvttpd2uqq xmm4, oword [eax+1]
	vcvttpd2uqq xmm4, oword [eax+64]
	vcvttpd2uqq ymm0, yword [eax+1]
	vcvttpd2uqq ymm0, yword [eax+64]
	vcvttpd2uqq zmm2, zword [eax+1]
	vcvttpd2uqq zmm0, zword [eax+64]
	vcvttpd2uqq xmm3, [0xf2d]
	vcvttpd2uqq ymm2, [0xdee]
	vcvttpd2uqq zmm6, [0x9ef]
