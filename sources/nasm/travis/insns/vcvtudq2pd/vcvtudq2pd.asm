default rel
	vcvtudq2pd xmm3, xmm2
	vcvtudq2pd xmm7, qword [0x9b5]
	vcvtudq2pd ymm1, xmm5
	vcvtudq2pd ymm5, xmm5
	vcvtudq2pd zmm4, ymm0
	vcvtudq2pd zmm1, ymm4
	vcvtudq2pd xmm8, xmm10
	vcvtudq2pd ymm11, xmm15
	vcvtudq2pd zmm8, ymm14
	vcvtudq2pd xmm28, xmm20
	vcvtudq2pd ymm25, xmm28
	vcvtudq2pd zmm23, ymm26
	vcvtudq2pd xmm5{k7}, xmm0
	vcvtudq2pd ymm5{k6}, xmm7
	vcvtudq2pd zmm0{k4}, ymm6
	vcvtudq2pd xmm6{k2}{z}, qword [0x911]
	vcvtudq2pd ymm2{k3}{z}, xmm3
	vcvtudq2pd zmm6{k6}{z}, ymm4
	vcvtudq2pd zmm6, ymm2, {rn-sae}
	vcvtudq2pd xmm1, qword [eax+1]
	vcvtudq2pd xmm0, qword [eax+64]
	vcvtudq2pd ymm1, oword [eax+1]
	vcvtudq2pd ymm5, oword [eax+64]
	vcvtudq2pd zmm3, yword [eax+1]
	vcvtudq2pd zmm7, yword [eax+64]
	vcvtudq2pd xmm2, [0x6c4]
	vcvtudq2pd ymm4, [0x5bb]
	vcvtudq2pd zmm1, [0x482]
