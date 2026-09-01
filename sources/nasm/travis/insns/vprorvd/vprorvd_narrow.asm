	vprorvd xmm6, xmm0, xmm5
	vprorvd xmm6, xmm5
	vprorvd xmm0, xmm1, xmm0
	vprorvd ymm4, ymm3, ymm6
	vprorvd ymm4, ymm6
	vprorvd ymm4, ymm3, ymm2
	vprorvd zmm2, zmm3, zmm1
	vprorvd zmm2, zmm1
	vprorvd zmm1, zmm2, zmm3

%ifdef ERROR
	vprorvd xmm10, xmm11, xmm12
	vprorvd ymm13, ymm8, ymm13
	vprorvd zmm15, zmm15, zmm15
	vprorvd xmm16, xmm27, xmm22
	vprorvd ymm31, ymm29, ymm19
	vprorvd zmm26, zmm16, zmm20
%endif
