default rel
	vcvtqq2pd xmm5, xmm6
	vcvtqq2pd xmm4, xmm2
	vcvtqq2pd ymm6, ymm0
	vcvtqq2pd ymm6, ymm3
	vcvtqq2pd zmm1, zword [0x37f]
	vcvtqq2pd zmm2, zmm4
	vcvtqq2pd xmm15, xmm12
	vcvtqq2pd ymm15, ymm10
	vcvtqq2pd zmm10, zmm8
	vcvtqq2pd xmm21, xmm16
	vcvtqq2pd ymm25, ymm28
	vcvtqq2pd zmm17, zmm29
	vcvtqq2pd xmm4{k2}, xmm5
	vcvtqq2pd ymm3{k7}, ymm0
	vcvtqq2pd zmm6{k6}, zmm7
	vcvtqq2pd xmm5{k7}{z}, xmm7
	vcvtqq2pd ymm5{k2}{z}, ymm7
	vcvtqq2pd zmm4{k4}{z}, zmm0
	vcvtqq2pd zmm2, zmm1, {rz-sae}
	vcvtqq2pd xmm5, oword [eax+1]
	vcvtqq2pd xmm2, oword [eax+64]
	vcvtqq2pd ymm3, yword [eax+1]
	vcvtqq2pd ymm6, yword [eax+64]
	vcvtqq2pd zmm7, zword [eax+1]
	vcvtqq2pd zmm7, zword [eax+64]
	vcvtqq2pd xmm7, [0xa70]
	vcvtqq2pd ymm2, [0xd92]
	vcvtqq2pd zmm5, [0x754]
