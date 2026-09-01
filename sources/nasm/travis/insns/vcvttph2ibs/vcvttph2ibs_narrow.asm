	vcvttph2ibs xmm0, oword [0xb5b]
	vcvttph2ibs xmm7, oword [0x342]
	vcvttph2ibs ymm7, ymm6
	vcvttph2ibs ymm7, yword [0x4dc]
	vcvttph2ibs zmm5, zmm7
	vcvttph2ibs zmm4, zword [0xac9]

%ifdef ERROR
	vcvttph2ibs xmm11, xmm12
	vcvttph2ibs ymm9, ymm15
	vcvttph2ibs zmm9, zmm8
	vcvttph2ibs xmm23, xmm30
	vcvttph2ibs ymm24, ymm18
	vcvttph2ibs zmm28, zmm24
%endif
