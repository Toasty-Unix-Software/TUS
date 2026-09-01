default rel
	vcvttpd2udqs xmm7, xmm3
	vcvttpd2udqs xmm6, xmm0
	vcvttpd2udqs xmm7, ymm1
	vcvttpd2udqs xmm4, yword [0xf3e]
	vcvttpd2udqs ymm7, zmm0
	vcvttpd2udqs ymm2, zmm1
	vcvttpd2udqs xmm8, xmm8
	vcvttpd2udqs xmm13, ymm14
	vcvttpd2udqs ymm10, zmm9
	vcvttpd2udqs xmm22, xmm18
	vcvttpd2udqs xmm16, ymm23
	vcvttpd2udqs ymm16, zmm22
	vcvttpd2udqs xmm5{k1}, oword [0x363]
	vcvttpd2udqs xmm1{k2}, ymm4
	vcvttpd2udqs ymm5{k5}, zmm1
	vcvttpd2udqs xmm3{k3}{z}, xmm6
	vcvttpd2udqs xmm3{k5}{z}, ymm7
	vcvttpd2udqs ymm2{k6}{z}, zmm6
	vcvttpd2udqs ymm3, zmm6, {sae}
	vcvttpd2udqs xmm5, oword [eax+1]
	vcvttpd2udqs xmm0, oword [eax+64]
	vcvttpd2udqs xmm7, yword [eax+1]
	vcvttpd2udqs xmm7, yword [eax+64]
	vcvttpd2udqs ymm0, zword [eax+1]
	vcvttpd2udqs ymm7, zword [eax+64]
	vcvttpd2udqs xmm6, [0x13f]
	vcvttpd2udqs xmm0, [0xb2c]
	vcvttpd2udqs ymm4, [0x8d4]
