	vshuff32x4 ymm3, ymm6, ymm0, 0xa9
	vshuff32x4 ymm3, ymm0, 0xa9
	vshuff32x4 ymm5, ymm4, ymm1, 0xda
	vshuff32x4 zmm1, zmm7, zmm6, 0xec
	vshuff32x4 zmm1, zmm6, 0xec
	vshuff32x4 zmm4, zmm1, zmm0, 0xa9

%ifdef ERROR
	vshuff32x4 ymm9, ymm9, ymm10, 0xe9
	vshuff32x4 zmm8, zmm9, zmm9, 0x5d
	vshuff32x4 ymm20, ymm27, ymm18, 0x79
	vshuff32x4 zmm28, zmm20, zmm17, 0x95
%endif
