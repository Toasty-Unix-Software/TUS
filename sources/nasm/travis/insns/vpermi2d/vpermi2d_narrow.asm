	vpermi2d xmm4, xmm2, xmm5
	vpermi2d xmm1, xmm1, xmm4
	vpermi2d ymm6, ymm5, yword [0xb13]
	vpermi2d ymm5, ymm0, ymm4
	vpermi2d zmm1, zmm6, zmm2
	vpermi2d zmm4, zmm3, zmm2

%ifdef ERROR
	vpermi2d xmm10, xmm8, xmm13
	vpermi2d ymm10, ymm11, ymm10
	vpermi2d zmm10, zmm10, zmm8
	vpermi2d xmm30, xmm23, xmm28
	vpermi2d ymm16, ymm29, ymm26
	vpermi2d zmm20, zmm16, zmm16
%endif
