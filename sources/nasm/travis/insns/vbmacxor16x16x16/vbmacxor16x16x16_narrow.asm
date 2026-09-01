	vbmacxor16x16x16 ymm6, ymm3, yword [0xeb6]
	vbmacxor16x16x16 ymm4, ymm1, yword [0xba6]
	vbmacxor16x16x16 zmm2, zmm1, zword [0xe77]
	vbmacxor16x16x16 zmm7, zmm1, zmm6

%ifdef ERROR
	vbmacxor16x16x16 ymm13, ymm10, ymm10
	vbmacxor16x16x16 zmm9, zmm10, zmm10
	vbmacxor16x16x16 ymm24, ymm17, ymm20
	vbmacxor16x16x16 zmm29, zmm26, zmm21
%endif
