default rel
	vmulbf16 xmm2, xmm2, xmm1
	vmulbf16 xmm7, xmm1, xmm3
	vmulbf16 ymm6, ymm2, ymm3
	vmulbf16 ymm3, ymm7, yword [0x6b2]
	vmulbf16 zmm6, zmm4, zword [0xc48]
	vmulbf16 zmm2, zmm1, zmm6
	vmulbf16 xmm14, xmm12, xmm14
	vmulbf16 ymm12, ymm11, ymm14
	vmulbf16 zmm11, zmm11, zmm13
	vmulbf16 xmm28, xmm28, xmm27
	vmulbf16 ymm24, ymm28, ymm31
	vmulbf16 zmm19, zmm24, zmm26
	vmulbf16 xmm0{k5}, xmm3, oword [0xff0]
	vmulbf16 ymm1{k3}, ymm1, ymm7
	vmulbf16 zmm7{k4}, zmm7, zmm6
	vmulbf16 xmm7{k6}{z}, xmm0, oword [0x9da]
	vmulbf16 ymm5{k7}{z}, ymm2, ymm3
	vmulbf16 zmm4{k3}{z}, zmm1, zmm0
	vmulbf16 xmm2, xmm2, oword [eax+1]
	vmulbf16 xmm5, xmm5, oword [eax+64]
	vmulbf16 ymm4, ymm0, yword [eax+1]
	vmulbf16 ymm0, ymm6, yword [eax+64]
	vmulbf16 zmm3, zmm5, zword [eax+1]
	vmulbf16 zmm0, zmm6, zword [eax+64]
	vmulbf16 xmm1, xmm0, [0x236]
	vmulbf16 ymm2, ymm5, [0x646]
	vmulbf16 zmm6, zmm7, [0x6b3]
