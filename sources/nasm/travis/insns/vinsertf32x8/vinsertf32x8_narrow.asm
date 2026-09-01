	vinsertf32x8 zmm0, zmm6, ymm2, 0xd1
	vinsertf32x8 zmm0, ymm2, 0xd1
	vinsertf32x8 zmm3, zmm2, ymm5, 0xd2

%ifdef ERROR
	vinsertf32x8 zmm12, zmm11, ymm11, 0xba
	vinsertf32x8 zmm17, zmm29, ymm18, 0x12
%endif
