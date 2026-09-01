	vinserti64x4 zmm7, zmm0, ymm1, 0xfa
	vinserti64x4 zmm7, ymm1, 0xfa
	vinserti64x4 zmm5, zmm5, ymm1, 0xba

%ifdef ERROR
	vinserti64x4 zmm15, zmm8, ymm8, 0x7b
	vinserti64x4 zmm30, zmm25, ymm17, 0xf6
%endif
