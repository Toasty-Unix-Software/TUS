	vcvtph2ibs xmm6, xmm2
	vcvtph2ibs xmm1, xmm4
	vcvtph2ibs ymm5, yword [0xded]
	vcvtph2ibs ymm6, ymm1
	vcvtph2ibs zmm4, zword [0x81d]
	vcvtph2ibs zmm1, zmm5

%ifdef ERROR
	vcvtph2ibs xmm13, xmm11
	vcvtph2ibs ymm13, ymm14
	vcvtph2ibs zmm14, zmm14
	vcvtph2ibs xmm31, xmm23
	vcvtph2ibs ymm20, ymm26
	vcvtph2ibs zmm20, zmm24
%endif
