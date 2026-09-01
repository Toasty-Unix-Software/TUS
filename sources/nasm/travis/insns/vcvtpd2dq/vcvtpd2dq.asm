default rel
	vcvtpd2dq xmm3, xmm5
	vcvtpd2dq xmm4, xmm5
	vcvtpd2dq xmm0, oword [0x413]
	vcvtpd2dq xmm6, oword [0x78b]
	vcvtpd2dq xmm7, ymm1
	vcvtpd2dq xmm3, ymm2
	vcvtpd2dq xmm5, yword [0x5e6]
	vcvtpd2dq xmm0, yword [0xfb3]
	vcvtpd2dq xmm15, xmm14
	vcvtpd2dq xmm9, oword [0xa0a]
	vcvtpd2dq xmm15, ymm12
	vcvtpd2dq xmm15, yword [0x3f6]
	vcvtpd2dq xmm16, xmm28
	vcvtpd2dq xmm29, oword [0xb06]
	vcvtpd2dq xmm26, ymm27
	vcvtpd2dq xmm19, yword [0x5e2]
	vcvtpd2dq xmm4{k1}, xmm1
	vcvtpd2dq xmm2{k5}, ymm7
	vcvtpd2dq ymm6{k2}, zmm4
	vcvtpd2dq xmm7{k6}{z}, oword [0x76a]
	vcvtpd2dq xmm0{k2}{z}, yword [0xc20]
	vcvtpd2dq ymm4{k2}{z}, zword [0xf7a]
	vcvtpd2dq ymm2, zmm5, {rn-sae}
	vcvtpd2dq xmm6, oword [eax+1]
	vcvtpd2dq xmm1, oword [eax+64]
	vcvtpd2dq xmm1, yword [eax+1]
	vcvtpd2dq xmm2, yword [eax+64]
	vcvtpd2dq xmm6, oword [eax+1]
	vcvtpd2dq xmm2, oword [eax+64]
	vcvtpd2dq xmm2, yword [eax+1]
	vcvtpd2dq xmm1, yword [eax+64]
	vcvtpd2dq ymm3, zword [eax+1]
	vcvtpd2dq ymm3, zword [eax+64]
	vcvtpd2dq xmm6, [0x5e1]
	vcvtpd2dq xmm7, [0x9b6]
	vcvtpd2dq xmm5, [0xbca]
	vcvtpd2dq xmm5, [0x1dd]
	vcvtpd2dq ymm4, [0x12b]
