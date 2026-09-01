default rel
	vcvtudq2ph xmm7, oword [0x3d9]
	vcvtudq2ph xmm5, xmm6
	vcvtudq2ph xmm7, ymm5
	vcvtudq2ph xmm7, ymm4
	vcvtudq2ph ymm1, zmm1
	vcvtudq2ph ymm2, zmm3
	vcvtudq2ph xmm13, xmm15
	vcvtudq2ph xmm15, ymm10
	vcvtudq2ph ymm14, zmm13
	vcvtudq2ph xmm31, xmm26
	vcvtudq2ph xmm19, ymm17
	vcvtudq2ph ymm23, zmm16
	vcvtudq2ph xmm5{k6}, xmm2
	vcvtudq2ph xmm3{k4}, ymm5
	vcvtudq2ph ymm5{k1}, zword [0x13e]
	vcvtudq2ph xmm3{k3}{z}, xmm1
	vcvtudq2ph xmm4{k2}{z}, ymm6
	vcvtudq2ph ymm1{k7}{z}, zmm2
	vcvtudq2ph xmm2, oword [eax+1]
	vcvtudq2ph xmm6, oword [eax+64]
	vcvtudq2ph xmm3, yword [eax+1]
	vcvtudq2ph xmm0, yword [eax+64]
	vcvtudq2ph ymm7, zword [eax+1]
	vcvtudq2ph ymm1, zword [eax+64]
	vcvtudq2ph xmm7, [0xecd]
	vcvtudq2ph xmm3, [0x787]
	vcvtudq2ph ymm1, [0x4b8]
