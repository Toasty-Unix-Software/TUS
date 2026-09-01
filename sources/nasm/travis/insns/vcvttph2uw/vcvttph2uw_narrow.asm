	vcvttph2uw xmm1, oword [0x36d]
	vcvttph2uw xmm0, xmm7
	vcvttph2uw ymm5, ymm2
	vcvttph2uw ymm3, yword [0x1d9]
	vcvttph2uw zmm2, zmm0
	vcvttph2uw zmm6, zmm1

%ifdef ERROR
	vcvttph2uw xmm8, xmm9
	vcvttph2uw ymm14, ymm10
	vcvttph2uw zmm15, zmm9
	vcvttph2uw xmm20, xmm31
	vcvttph2uw ymm23, ymm21
	vcvttph2uw zmm22, zmm26
%endif
