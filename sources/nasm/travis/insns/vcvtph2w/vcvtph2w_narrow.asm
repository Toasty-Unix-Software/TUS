	vcvtph2w xmm7, xmm4
	vcvtph2w xmm3, xmm7
	vcvtph2w ymm0, yword [0x161]
	vcvtph2w ymm6, ymm6
	vcvtph2w zmm3, zword [0x101]
	vcvtph2w zmm0, zword [0x5cc]

%ifdef ERROR
	vcvtph2w xmm11, xmm9
	vcvtph2w ymm11, ymm13
	vcvtph2w zmm10, zmm14
	vcvtph2w xmm25, xmm30
	vcvtph2w ymm28, ymm31
	vcvtph2w zmm28, zmm30
%endif
