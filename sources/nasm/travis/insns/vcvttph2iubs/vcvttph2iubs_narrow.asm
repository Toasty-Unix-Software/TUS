	vcvttph2iubs xmm4, xmm1
	vcvttph2iubs xmm3, xmm5
	vcvttph2iubs ymm0, ymm1
	vcvttph2iubs ymm7, ymm6
	vcvttph2iubs zmm1, zword [0x4f7]
	vcvttph2iubs zmm0, zmm4

%ifdef ERROR
	vcvttph2iubs xmm8, xmm8
	vcvttph2iubs ymm14, ymm8
	vcvttph2iubs zmm12, zmm14
	vcvttph2iubs xmm21, xmm20
	vcvttph2iubs ymm26, ymm20
	vcvttph2iubs zmm23, zmm26
%endif
