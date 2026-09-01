default rel
	vcvttps2qq xmm2, qword [0xc06]
	vcvttps2qq xmm1, xmm7
	vcvttps2qq ymm6, xmm0
	vcvttps2qq ymm2, oword [0x32a]
	vcvttps2qq zmm0, ymm0
	vcvttps2qq zmm5, ymm5
	vcvttps2qq xmm14, xmm13
	vcvttps2qq ymm10, xmm11
	vcvttps2qq zmm13, ymm9
	vcvttps2qq xmm17, xmm26
	vcvttps2qq ymm18, xmm29
	vcvttps2qq zmm23, ymm27
	vcvttps2qq xmm1{k3}, qword [0x178]
	vcvttps2qq ymm0{k6}, xmm5
	vcvttps2qq zmm2{k6}, ymm7
	vcvttps2qq xmm2{k2}{z}, xmm5
	vcvttps2qq ymm1{k1}{z}, xmm5
	vcvttps2qq zmm0{k3}{z}, yword [0xedd]
	vcvttps2qq zmm5, ymm6, {sae}
	vcvttps2qq xmm7, qword [eax+1]
	vcvttps2qq xmm0, qword [eax+64]
	vcvttps2qq ymm3, oword [eax+1]
	vcvttps2qq ymm5, oword [eax+64]
	vcvttps2qq zmm3, yword [eax+1]
	vcvttps2qq zmm3, yword [eax+64]
	vcvttps2qq xmm0, [0x3ed]
	vcvttps2qq ymm2, [0xe7f]
	vcvttps2qq zmm3, [0xe88]
