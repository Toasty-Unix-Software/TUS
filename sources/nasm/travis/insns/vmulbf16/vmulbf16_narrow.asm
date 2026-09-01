	vmulbf16 xmm2, xmm2, xmm1
	vmulbf16 xmm7, xmm1, xmm3
	vmulbf16 ymm6, ymm2, ymm3
	vmulbf16 ymm3, ymm7, yword [0x6b2]
	vmulbf16 zmm6, zmm4, zword [0xc48]
	vmulbf16 zmm2, zmm1, zmm6

%ifdef ERROR
	vmulbf16 xmm14, xmm12, xmm14
	vmulbf16 ymm12, ymm11, ymm14
	vmulbf16 zmm11, zmm11, zmm13
	vmulbf16 xmm28, xmm28, xmm27
	vmulbf16 ymm24, ymm28, ymm31
	vmulbf16 zmm19, zmm24, zmm26
%endif
