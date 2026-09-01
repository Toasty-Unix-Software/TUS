	vshufi32x4 ymm3, ymm1, yword [0x38c], 0xd7
	vshufi32x4 ymm3, yword [0x38c], 0xd7
	vshufi32x4 ymm4, ymm6, ymm2, 0x3a
	vshufi32x4 zmm2, zmm3, zword [0xadd], 0x30
	vshufi32x4 zmm2, zword [0xadd], 0x30
	vshufi32x4 zmm7, zmm2, zmm1, 0x43

%ifdef ERROR
	vshufi32x4 ymm14, ymm14, ymm8, 0x91
	vshufi32x4 zmm10, zmm13, zmm13, 0xd3
	vshufi32x4 ymm30, ymm17, ymm20, 0x69
	vshufi32x4 zmm17, zmm18, zmm30, 0xf3
%endif
