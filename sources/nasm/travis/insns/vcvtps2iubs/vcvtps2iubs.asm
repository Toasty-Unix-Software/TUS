default rel
	vcvtps2iubs xmm7, xmm1
	vcvtps2iubs xmm0, oword [0x9fc]
	vcvtps2iubs ymm7, ymm5
	vcvtps2iubs ymm0, ymm6
	vcvtps2iubs zmm0, zmm4
	vcvtps2iubs zmm7, zmm4
	vcvtps2iubs xmm8, xmm15
	vcvtps2iubs ymm14, ymm13
	vcvtps2iubs zmm15, zmm15
	vcvtps2iubs xmm20, xmm28
	vcvtps2iubs ymm29, ymm19
	vcvtps2iubs zmm25, zmm20
	vcvtps2iubs xmm4{k3}, xmm2
	vcvtps2iubs ymm7{k1}, yword [0x377]
	vcvtps2iubs zmm6{k3}, zmm1
	vcvtps2iubs xmm7{k7}{z}, xmm7
	vcvtps2iubs ymm3{k1}{z}, ymm4
	vcvtps2iubs zmm0{k3}{z}, zmm5
	vcvtps2iubs zmm7, zmm1, {rn-sae}
	vcvtps2iubs xmm3, oword [eax+1]
	vcvtps2iubs xmm4, oword [eax+64]
	vcvtps2iubs ymm5, yword [eax+1]
	vcvtps2iubs ymm4, yword [eax+64]
	vcvtps2iubs zmm0, zword [eax+1]
	vcvtps2iubs zmm2, zword [eax+64]
	vcvtps2iubs xmm5, [0xfed]
	vcvtps2iubs ymm4, [0x866]
	vcvtps2iubs zmm0, [0x539]
