	vshuff64x2 ymm1, ymm7, yword [0x2f7], 0x98
	vshuff64x2 ymm1, yword [0x2f7], 0x98
	vshuff64x2 ymm7, ymm1, ymm6, 0x29
	vshuff64x2 zmm5, zmm6, zmm1, 0x3c
	vshuff64x2 zmm5, zmm1, 0x3c
	vshuff64x2 zmm1, zmm6, zmm6, 0xd4

%ifdef ERROR
	vshuff64x2 ymm15, ymm11, ymm14, 0xfd
	vshuff64x2 zmm15, zmm14, zmm9, 0x57
	vshuff64x2 ymm20, ymm24, ymm17, 0x15
	vshuff64x2 zmm21, zmm19, zmm18, 0xf2
%endif
