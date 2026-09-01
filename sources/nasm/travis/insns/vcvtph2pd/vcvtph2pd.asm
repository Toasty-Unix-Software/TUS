default rel
	vcvtph2pd xmm5, dword [0x3c4]
	vcvtph2pd xmm4, xmm4
	vcvtph2pd ymm6, xmm2
	vcvtph2pd ymm2, xmm7
	vcvtph2pd zmm6, xmm2
	vcvtph2pd zmm5, xmm3
	vcvtph2pd xmm10, xmm12
	vcvtph2pd ymm10, xmm10
	vcvtph2pd zmm14, xmm15
	vcvtph2pd xmm18, xmm25
	vcvtph2pd ymm31, xmm16
	vcvtph2pd zmm26, xmm16
	vcvtph2pd xmm4{k1}, xmm1
	vcvtph2pd ymm2{k3}, xmm0
	vcvtph2pd zmm1{k5}, oword [0x937]
	vcvtph2pd xmm3{k2}{z}, xmm1
	vcvtph2pd ymm5{k3}{z}, xmm5
	vcvtph2pd zmm3{k6}{z}, xmm2
	vcvtph2pd zmm1, xmm0, {sae}
	vcvtph2pd xmm5, dword [eax+1]
	vcvtph2pd xmm6, dword [eax+64]
	vcvtph2pd ymm7, qword [eax+1]
	vcvtph2pd ymm5, qword [eax+64]
	vcvtph2pd zmm7, oword [eax+1]
	vcvtph2pd zmm2, oword [eax+64]
	vcvtph2pd xmm5, [0x7a6]
	vcvtph2pd ymm3, [0xf02]
	vcvtph2pd zmm5, [0x9de]
