default rel
	vcvttpd2dqs xmm6, xmm1
	vcvttpd2dqs xmm1, xmm0
	vcvttpd2dqs xmm5, yword [0x240]
	vcvttpd2dqs xmm5, ymm6
	vcvttpd2dqs ymm5, zmm3
	vcvttpd2dqs ymm6, zmm4
	vcvttpd2dqs xmm14, xmm9
	vcvttpd2dqs xmm8, ymm13
	vcvttpd2dqs ymm12, zmm10
	vcvttpd2dqs xmm31, xmm23
	vcvttpd2dqs xmm21, ymm22
	vcvttpd2dqs ymm25, zmm31
	vcvttpd2dqs xmm7{k2}, oword [0xe64]
	vcvttpd2dqs xmm3{k1}, yword [0x637]
	vcvttpd2dqs ymm2{k6}, zmm5
	vcvttpd2dqs xmm1{k1}{z}, xmm1
	vcvttpd2dqs xmm2{k6}{z}, yword [0xa1d]
	vcvttpd2dqs ymm4{k6}{z}, zmm4
	vcvttpd2dqs ymm6, zmm4, {sae}
	vcvttpd2dqs xmm3, oword [eax+1]
	vcvttpd2dqs xmm4, oword [eax+64]
	vcvttpd2dqs xmm4, yword [eax+1]
	vcvttpd2dqs xmm5, yword [eax+64]
	vcvttpd2dqs ymm3, zword [eax+1]
	vcvttpd2dqs ymm6, zword [eax+64]
	vcvttpd2dqs xmm2, [0xfe4]
	vcvttpd2dqs xmm5, [0x38c]
	vcvttpd2dqs ymm5, [0x872]
