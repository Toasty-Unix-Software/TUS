default rel
	vcvttbf162iubs xmm3, oword [0x5e0]
	vcvttbf162iubs xmm3, oword [0x1e8]
	vcvttbf162iubs ymm3, ymm2
	vcvttbf162iubs ymm3, yword [0x6b9]
	vcvttbf162iubs zmm6, zmm6
	vcvttbf162iubs zmm0, zmm4
	vcvttbf162iubs xmm13, xmm11
	vcvttbf162iubs ymm12, ymm9
	vcvttbf162iubs zmm13, zmm8
	vcvttbf162iubs xmm16, xmm30
	vcvttbf162iubs ymm30, ymm16
	vcvttbf162iubs zmm18, zmm23
	vcvttbf162iubs xmm7{k3}, xmm4
	vcvttbf162iubs ymm3{k1}, ymm2
	vcvttbf162iubs zmm5{k4}, zmm6
	vcvttbf162iubs xmm2{k6}{z}, xmm6
	vcvttbf162iubs ymm4{k6}{z}, yword [0xd17]
	vcvttbf162iubs zmm5{k4}{z}, zmm6
	vcvttbf162iubs xmm1, oword [eax+1]
	vcvttbf162iubs xmm5, oword [eax+64]
	vcvttbf162iubs ymm5, yword [eax+1]
	vcvttbf162iubs ymm5, yword [eax+64]
	vcvttbf162iubs zmm0, zword [eax+1]
	vcvttbf162iubs zmm7, zword [eax+64]
	vcvttbf162iubs xmm1, [0xb36]
	vcvttbf162iubs ymm0, [0x76f]
	vcvttbf162iubs zmm6, [0xff1]
