default rel
	vcvtudq2ps xmm1, xmm5
	vcvtudq2ps xmm5, oword [0xeef]
	vcvtudq2ps ymm1, ymm6
	vcvtudq2ps ymm5, ymm6
	vcvtudq2ps zmm0, zmm4
	vcvtudq2ps zmm0, zmm0
	vcvtudq2ps xmm9, xmm11
	vcvtudq2ps ymm13, ymm13
	vcvtudq2ps zmm9, zmm8
	vcvtudq2ps xmm29, xmm24
	vcvtudq2ps ymm23, ymm25
	vcvtudq2ps zmm16, zmm19
	vcvtudq2ps xmm7{k7}, oword [0x3be]
	vcvtudq2ps ymm0{k2}, ymm4
	vcvtudq2ps zmm4{k1}, zword [0x617]
	vcvtudq2ps xmm2{k1}{z}, xmm1
	vcvtudq2ps ymm4{k6}{z}, ymm0
	vcvtudq2ps zmm5{k5}{z}, zmm4
	vcvtudq2ps zmm6, zmm7, {rn-sae}
	vcvtudq2ps xmm5, oword [eax+1]
	vcvtudq2ps xmm3, oword [eax+64]
	vcvtudq2ps ymm5, yword [eax+1]
	vcvtudq2ps ymm2, yword [eax+64]
	vcvtudq2ps zmm1, zword [eax+1]
	vcvtudq2ps zmm5, zword [eax+64]
	vcvtudq2ps xmm4, [0xf1c]
	vcvtudq2ps ymm0, [0xcdb]
	vcvtudq2ps zmm4, [0xeb1]
