	vcvtudq2ph xmm7, oword [0x3d9]
	vcvtudq2ph xmm5, xmm6
	vcvtudq2ph xmm7, ymm5
	vcvtudq2ph xmm7, ymm4
	vcvtudq2ph ymm1, zmm1
	vcvtudq2ph ymm2, zmm3

%ifdef ERROR
	vcvtudq2ph xmm13, xmm15
	vcvtudq2ph xmm15, ymm10
	vcvtudq2ph ymm14, zmm13
	vcvtudq2ph xmm31, xmm26
	vcvtudq2ph xmm19, ymm17
	vcvtudq2ph ymm23, zmm16
%endif
