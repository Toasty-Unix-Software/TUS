default rel
	vcvttph2iubs xmm4, xmm1
	vcvttph2iubs xmm3, xmm5
	vcvttph2iubs ymm0, ymm1
	vcvttph2iubs ymm7, ymm6
	vcvttph2iubs zmm1, zword [0x4f7]
	vcvttph2iubs zmm0, zmm4
	vcvttph2iubs xmm8, xmm8
	vcvttph2iubs ymm14, ymm8
	vcvttph2iubs zmm12, zmm14
	vcvttph2iubs xmm21, xmm20
	vcvttph2iubs ymm26, ymm20
	vcvttph2iubs zmm23, zmm26
	vcvttph2iubs xmm7{k4}, xmm3
	vcvttph2iubs ymm4{k2}, ymm7
	vcvttph2iubs zmm3{k3}, zmm0
	vcvttph2iubs xmm2{k1}{z}, xmm3
	vcvttph2iubs ymm2{k4}{z}, ymm1
	vcvttph2iubs zmm2{k1}{z}, zword [0x1bd]
	vcvttph2iubs zmm5, zmm4, {sae}
	vcvttph2iubs xmm6, oword [eax+1]
	vcvttph2iubs xmm4, oword [eax+64]
	vcvttph2iubs ymm2, yword [eax+1]
	vcvttph2iubs ymm4, yword [eax+64]
	vcvttph2iubs zmm3, zword [eax+1]
	vcvttph2iubs zmm0, zword [eax+64]
	vcvttph2iubs xmm3, [0x48b]
	vcvttph2iubs ymm3, [0xe47]
	vcvttph2iubs zmm5, [0xb39]
