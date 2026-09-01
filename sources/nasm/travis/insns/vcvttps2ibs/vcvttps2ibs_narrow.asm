	vcvttps2ibs xmm0, oword [0x657]
	vcvttps2ibs xmm3, oword [0x61a]
	vcvttps2ibs ymm3, ymm1
	vcvttps2ibs ymm7, ymm4
	vcvttps2ibs zmm5, zword [0xc95]
	vcvttps2ibs zmm0, zmm5

%ifdef ERROR
	vcvttps2ibs xmm8, xmm11
	vcvttps2ibs ymm9, ymm14
	vcvttps2ibs zmm11, zmm9
	vcvttps2ibs xmm17, xmm25
	vcvttps2ibs ymm20, ymm29
	vcvttps2ibs zmm31, zmm19
%endif
