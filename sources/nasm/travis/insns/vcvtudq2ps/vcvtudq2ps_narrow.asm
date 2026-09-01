	vcvtudq2ps xmm1, xmm5
	vcvtudq2ps xmm5, oword [0xeef]
	vcvtudq2ps ymm1, ymm6
	vcvtudq2ps ymm5, ymm6
	vcvtudq2ps zmm0, zmm4
	vcvtudq2ps zmm0, zmm0

%ifdef ERROR
	vcvtudq2ps xmm9, xmm11
	vcvtudq2ps ymm13, ymm13
	vcvtudq2ps zmm9, zmm8
	vcvtudq2ps xmm29, xmm24
	vcvtudq2ps ymm23, ymm25
	vcvtudq2ps zmm16, zmm19
%endif
