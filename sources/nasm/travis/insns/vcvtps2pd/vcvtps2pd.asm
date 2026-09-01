default rel
	vcvtps2pd xmm4, xmm0
	vcvtps2pd xmm6, xmm5
	vcvtps2pd ymm0, xmm4
	vcvtps2pd ymm0, oword [0xcc5]
	vcvtps2pd xmm1, xmm4
	vcvtps2pd xmm4, xmm0
	vcvtps2pd ymm5, xmm2
	vcvtps2pd ymm3, oword [0xdd3]
	vcvtps2pd xmm13, xmm11
	vcvtps2pd ymm13, xmm12
	vcvtps2pd xmm8, xmm15
	vcvtps2pd ymm11, xmm8
	vcvtps2pd xmm31, xmm18
	vcvtps2pd ymm23, xmm24
	vcvtps2pd xmm27, xmm22
	vcvtps2pd ymm25, xmm25
	vcvtps2pd xmm6{k7}, qword [0x34c]
	vcvtps2pd ymm3{k1}, xmm6
	vcvtps2pd zmm4{k7}, ymm0
	vcvtps2pd xmm5{k6}{z}, qword [0xebd]
	vcvtps2pd ymm2{k1}{z}, xmm1
	vcvtps2pd zmm1{k6}{z}, ymm5
	vcvtps2pd zmm1, ymm2, {sae}
	vcvtps2pd xmm7, qword [eax+1]
	vcvtps2pd xmm0, qword [eax+64]
	vcvtps2pd ymm6, oword [eax+1]
	vcvtps2pd ymm5, oword [eax+64]
	vcvtps2pd xmm0, qword [eax+1]
	vcvtps2pd xmm4, qword [eax+64]
	vcvtps2pd ymm5, oword [eax+1]
	vcvtps2pd ymm6, oword [eax+64]
	vcvtps2pd zmm6, yword [eax+1]
	vcvtps2pd zmm0, yword [eax+64]
	vcvtps2pd xmm4, [0x33a]
	vcvtps2pd ymm6, [0x373]
	vcvtps2pd xmm4, [0xb9e]
	vcvtps2pd ymm5, [0xae1]
	vcvtps2pd zmm3, [0x909]
