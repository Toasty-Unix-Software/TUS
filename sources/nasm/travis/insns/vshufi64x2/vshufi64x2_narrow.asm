	vshufi64x2 ymm0, ymm5, yword [0x510], 0x14
	vshufi64x2 ymm0, yword [0x510], 0x14
	vshufi64x2 ymm5, ymm1, yword [0x201], 0xfe
	vshufi64x2 zmm7, zmm4, zmm7, 0xf4
	vshufi64x2 zmm7, zmm7, 0xf4
	vshufi64x2 zmm5, zmm6, zmm3, 0xf0

%ifdef ERROR
	vshufi64x2 ymm11, ymm9, ymm13, 0x1e
	vshufi64x2 zmm10, zmm14, zmm14, 0x3c
	vshufi64x2 ymm30, ymm18, ymm17, 0x5b
	vshufi64x2 zmm26, zmm18, zmm22, 0xd0
%endif
