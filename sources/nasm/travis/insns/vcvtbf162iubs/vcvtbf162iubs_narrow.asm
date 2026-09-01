	vcvtbf162iubs xmm4, xmm4
	vcvtbf162iubs xmm0, xmm4
	vcvtbf162iubs ymm7, ymm0
	vcvtbf162iubs ymm4, ymm2
	vcvtbf162iubs zmm3, zword [0x182]
	vcvtbf162iubs zmm7, zmm3

%ifdef ERROR
	vcvtbf162iubs xmm12, xmm11
	vcvtbf162iubs ymm11, ymm11
	vcvtbf162iubs zmm13, zmm12
	vcvtbf162iubs xmm19, xmm30
	vcvtbf162iubs ymm30, ymm24
	vcvtbf162iubs zmm23, zmm31
%endif
