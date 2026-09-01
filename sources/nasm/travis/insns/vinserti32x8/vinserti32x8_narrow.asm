	vinserti32x8 zmm5, zmm3, ymm0, 0x17
	vinserti32x8 zmm5, ymm0, 0x17
	vinserti32x8 zmm2, zmm6, ymm5, 0x4c

%ifdef ERROR
	vinserti32x8 zmm15, zmm9, ymm8, 0x2d
	vinserti32x8 zmm16, zmm29, ymm23, 0xf1
%endif
