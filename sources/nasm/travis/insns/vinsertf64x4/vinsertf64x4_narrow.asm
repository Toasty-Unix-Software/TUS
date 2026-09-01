	vinsertf64x4 zmm4, zmm2, ymm2, 0x6a
	vinsertf64x4 zmm4, ymm2, 0x6a
	vinsertf64x4 zmm6, zmm4, ymm7, 0x23

%ifdef ERROR
	vinsertf64x4 zmm14, zmm13, ymm11, 0x8f
	vinsertf64x4 zmm27, zmm18, ymm24, 0x68
%endif
