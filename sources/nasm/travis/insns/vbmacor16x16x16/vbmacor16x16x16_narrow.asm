	vbmacor16x16x16 ymm5, ymm5, yword [0x2f3]
	vbmacor16x16x16 ymm1, ymm4, ymm5
	vbmacor16x16x16 zmm6, zmm3, zword [0x3c8]
	vbmacor16x16x16 zmm6, zmm0, zword [0x3fd]

%ifdef ERROR
	vbmacor16x16x16 ymm12, ymm9, ymm9
	vbmacor16x16x16 zmm9, zmm11, zmm14
	vbmacor16x16x16 ymm29, ymm24, ymm21
	vbmacor16x16x16 zmm28, zmm31, zmm19
%endif
