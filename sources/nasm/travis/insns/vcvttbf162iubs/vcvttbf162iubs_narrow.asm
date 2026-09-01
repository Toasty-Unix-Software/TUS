	vcvttbf162iubs xmm3, oword [0x5e0]
	vcvttbf162iubs xmm3, oword [0x1e8]
	vcvttbf162iubs ymm3, ymm2
	vcvttbf162iubs ymm3, yword [0x6b9]
	vcvttbf162iubs zmm6, zmm6
	vcvttbf162iubs zmm0, zmm4

%ifdef ERROR
	vcvttbf162iubs xmm13, xmm11
	vcvttbf162iubs ymm12, ymm9
	vcvttbf162iubs zmm13, zmm8
	vcvttbf162iubs xmm16, xmm30
	vcvttbf162iubs ymm30, ymm16
	vcvttbf162iubs zmm18, zmm23
%endif
