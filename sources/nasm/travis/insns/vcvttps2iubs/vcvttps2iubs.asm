default rel
	vcvttps2iubs xmm5, xmm2
	vcvttps2iubs xmm3, oword [0xce7]
	vcvttps2iubs ymm4, ymm1
	vcvttps2iubs ymm1, ymm0
	vcvttps2iubs zmm2, zmm5
	vcvttps2iubs zmm7, zmm3
	vcvttps2iubs xmm9, xmm9
	vcvttps2iubs ymm14, ymm8
	vcvttps2iubs zmm15, zmm15
	vcvttps2iubs xmm22, xmm28
	vcvttps2iubs ymm22, ymm26
	vcvttps2iubs zmm25, zmm31
	vcvttps2iubs xmm4{k5}, xmm6
	vcvttps2iubs ymm0{k1}, ymm6
	vcvttps2iubs zmm7{k3}, zmm0
	vcvttps2iubs xmm4{k2}{z}, xmm3
	vcvttps2iubs ymm6{k6}{z}, yword [0xbfd]
	vcvttps2iubs zmm1{k5}{z}, zmm1
	vcvttps2iubs zmm4, zmm7, {sae}
	vcvttps2iubs xmm0, oword [eax+1]
	vcvttps2iubs xmm0, oword [eax+64]
	vcvttps2iubs ymm2, yword [eax+1]
	vcvttps2iubs ymm4, yword [eax+64]
	vcvttps2iubs zmm2, zword [eax+1]
	vcvttps2iubs zmm4, zword [eax+64]
	vcvttps2iubs xmm3, [0xbed]
	vcvttps2iubs ymm7, [0x663]
	vcvttps2iubs zmm6, [0x81f]
