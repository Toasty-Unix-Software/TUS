default rel
	vcvttpd2qq xmm7, xmm1
	vcvttpd2qq xmm6, oword [0xdf5]
	vcvttpd2qq ymm5, yword [0xb7b]
	vcvttpd2qq ymm3, yword [0xbfe]
	vcvttpd2qq zmm0, zword [0x1cf]
	vcvttpd2qq zmm1, zmm7
	vcvttpd2qq xmm8, xmm8
	vcvttpd2qq ymm15, ymm11
	vcvttpd2qq zmm14, zmm15
	vcvttpd2qq xmm19, xmm26
	vcvttpd2qq ymm25, ymm17
	vcvttpd2qq zmm22, zmm21
	vcvttpd2qq xmm1{k1}, xmm0
	vcvttpd2qq ymm7{k6}, ymm1
	vcvttpd2qq zmm5{k3}, zword [0xfb0]
	vcvttpd2qq xmm6{k2}{z}, oword [0x1c8]
	vcvttpd2qq ymm4{k1}{z}, ymm3
	vcvttpd2qq zmm4{k2}{z}, zmm0
	vcvttpd2qq zmm1, zmm5, {sae}
	vcvttpd2qq xmm0, oword [eax+1]
	vcvttpd2qq xmm6, oword [eax+64]
	vcvttpd2qq ymm7, yword [eax+1]
	vcvttpd2qq ymm0, yword [eax+64]
	vcvttpd2qq zmm7, zword [eax+1]
	vcvttpd2qq zmm0, zword [eax+64]
	vcvttpd2qq xmm1, [0xa5d]
	vcvttpd2qq ymm5, [0x14d]
	vcvttpd2qq zmm5, [0xf09]
