default rel
	vcvttpd2dq xmm1, xmm1
	vcvttpd2dq xmm6, xmm5
	vcvttpd2dq xmm3, oword [0x961]
	vcvttpd2dq xmm1, oword [0x283]
	vcvttpd2dq xmm4, ymm6
	vcvttpd2dq xmm4, ymm4
	vcvttpd2dq xmm7, yword [0x7f1]
	vcvttpd2dq xmm6, yword [0xc06]
	vcvttpd2dq xmm11, xmm14
	vcvttpd2dq xmm14, oword [0xe11]
	vcvttpd2dq xmm10, ymm11
	vcvttpd2dq xmm13, yword [0x8b2]
	vcvttpd2dq xmm20, xmm18
	vcvttpd2dq xmm23, oword [0x63a]
	vcvttpd2dq xmm26, ymm28
	vcvttpd2dq xmm20, yword [0xf6c]
	vcvttpd2dq xmm6{k1}, xmm4
	vcvttpd2dq xmm1{k2}, ymm2
	vcvttpd2dq ymm7{k3}, zmm4
	vcvttpd2dq xmm4{k4}{z}, xmm7
	vcvttpd2dq xmm4{k1}{z}, ymm6
	vcvttpd2dq ymm5{k7}{z}, zword [0x5c3]
	vcvttpd2dq ymm3, zmm6, {sae}
	vcvttpd2dq xmm7, oword [eax+1]
	vcvttpd2dq xmm6, oword [eax+64]
	vcvttpd2dq xmm7, yword [eax+1]
	vcvttpd2dq xmm7, yword [eax+64]
	vcvttpd2dq xmm7, oword [eax+1]
	vcvttpd2dq xmm7, oword [eax+64]
	vcvttpd2dq xmm7, yword [eax+1]
	vcvttpd2dq xmm6, yword [eax+64]
	vcvttpd2dq ymm3, zword [eax+1]
	vcvttpd2dq ymm7, zword [eax+64]
	vcvttpd2dq xmm3, [0xf15]
	vcvttpd2dq xmm7, [0x911]
	vcvttpd2dq xmm2, [0x1f3]
	vcvttpd2dq xmm5, [0x58c]
	vcvttpd2dq ymm0, [0x1bb]
