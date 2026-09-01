default rel
	vcvttpd2qqs xmm3, oword [0xa5c]
	vcvttpd2qqs xmm7, xmm1
	vcvttpd2qqs ymm1, ymm1
	vcvttpd2qqs ymm1, ymm5
	vcvttpd2qqs zmm2, zword [0x81a]
	vcvttpd2qqs zmm5, zmm1
	vcvttpd2qqs xmm11, xmm12
	vcvttpd2qqs ymm10, ymm8
	vcvttpd2qqs zmm10, zmm14
	vcvttpd2qqs xmm23, xmm30
	vcvttpd2qqs ymm17, ymm21
	vcvttpd2qqs zmm20, zmm26
	vcvttpd2qqs xmm2{k6}, oword [0x719]
	vcvttpd2qqs ymm1{k4}, ymm0
	vcvttpd2qqs zmm2{k7}, zmm2
	vcvttpd2qqs xmm5{k3}{z}, xmm7
	vcvttpd2qqs ymm3{k6}{z}, ymm5
	vcvttpd2qqs zmm4{k3}{z}, zword [0x588]
	vcvttpd2qqs zmm0, zmm5, {sae}
	vcvttpd2qqs xmm6, oword [eax+1]
	vcvttpd2qqs xmm0, oword [eax+64]
	vcvttpd2qqs ymm7, yword [eax+1]
	vcvttpd2qqs ymm7, yword [eax+64]
	vcvttpd2qqs zmm0, zword [eax+1]
	vcvttpd2qqs zmm5, zword [eax+64]
	vcvttpd2qqs xmm5, [0xf24]
	vcvttpd2qqs ymm7, [0x1f5]
	vcvttpd2qqs zmm0, [0x981]
