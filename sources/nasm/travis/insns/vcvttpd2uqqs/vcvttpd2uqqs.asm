default rel
	vcvttpd2uqqs xmm1, xmm2
	vcvttpd2uqqs xmm1, xmm1
	vcvttpd2uqqs ymm0, ymm2
	vcvttpd2uqqs ymm3, ymm5
	vcvttpd2uqqs zmm4, zmm6
	vcvttpd2uqqs zmm2, zmm3
	vcvttpd2uqqs xmm10, xmm11
	vcvttpd2uqqs ymm12, ymm12
	vcvttpd2uqqs zmm14, zmm15
	vcvttpd2uqqs xmm22, xmm16
	vcvttpd2uqqs ymm23, ymm27
	vcvttpd2uqqs zmm22, zmm28
	vcvttpd2uqqs xmm4{k6}, xmm1
	vcvttpd2uqqs ymm2{k4}, ymm7
	vcvttpd2uqqs zmm4{k3}, zmm4
	vcvttpd2uqqs xmm4{k1}{z}, oword [0x9bb]
	vcvttpd2uqqs ymm7{k4}{z}, yword [0x24f]
	vcvttpd2uqqs zmm5{k1}{z}, zword [0x52e]
	vcvttpd2uqqs zmm0, zmm6, {sae}
	vcvttpd2uqqs xmm2, oword [eax+1]
	vcvttpd2uqqs xmm6, oword [eax+64]
	vcvttpd2uqqs ymm0, yword [eax+1]
	vcvttpd2uqqs ymm4, yword [eax+64]
	vcvttpd2uqqs zmm5, zword [eax+1]
	vcvttpd2uqqs zmm5, zword [eax+64]
	vcvttpd2uqqs xmm5, [0x701]
	vcvttpd2uqqs ymm3, [0x6cc]
	vcvttpd2uqqs zmm1, [0x508]
