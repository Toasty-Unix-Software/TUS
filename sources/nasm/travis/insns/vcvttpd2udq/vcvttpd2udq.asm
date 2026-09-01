default rel
	vcvttpd2udq xmm6, xmm3
	vcvttpd2udq xmm6, xmm2
	vcvttpd2udq xmm0, ymm5
	vcvttpd2udq xmm6, yword [0xd12]
	vcvttpd2udq ymm3, zword [0x5f5]
	vcvttpd2udq ymm0, zmm0
	vcvttpd2udq xmm8, xmm12
	vcvttpd2udq xmm11, ymm13
	vcvttpd2udq ymm9, zmm14
	vcvttpd2udq xmm19, xmm27
	vcvttpd2udq xmm26, ymm29
	vcvttpd2udq ymm19, zmm31
	vcvttpd2udq xmm5{k6}, xmm3
	vcvttpd2udq xmm2{k1}, yword [0xac0]
	vcvttpd2udq ymm3{k2}, zmm3
	vcvttpd2udq xmm2{k6}{z}, xmm3
	vcvttpd2udq xmm0{k6}{z}, yword [0xcf8]
	vcvttpd2udq ymm5{k7}{z}, zword [0x274]
	vcvttpd2udq ymm4, zmm3, {sae}
	vcvttpd2udq xmm7, oword [eax+1]
	vcvttpd2udq xmm5, oword [eax+64]
	vcvttpd2udq xmm3, yword [eax+1]
	vcvttpd2udq xmm2, yword [eax+64]
	vcvttpd2udq ymm0, zword [eax+1]
	vcvttpd2udq ymm2, zword [eax+64]
	vcvttpd2udq xmm6, [0x53b]
	vcvttpd2udq xmm6, [0x241]
	vcvttpd2udq ymm1, [0x21d]
