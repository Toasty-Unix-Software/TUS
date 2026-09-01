default rel
	vminbf16 xmm1, xmm4, xmm7
	vminbf16 xmm3, xmm5, xmm6
	vminbf16 ymm5, ymm3, yword [0xb5a]
	vminbf16 ymm6, ymm2, ymm4
	vminbf16 zmm4, zmm0, zmm1
	vminbf16 zmm2, zmm6, zmm2
	vminbf16 xmm8, xmm10, xmm13
	vminbf16 ymm15, ymm12, ymm8
	vminbf16 zmm10, zmm8, zmm13
	vminbf16 xmm20, xmm22, xmm24
	vminbf16 ymm20, ymm17, ymm29
	vminbf16 zmm17, zmm28, zmm31
	vminbf16 xmm4{k7}, xmm3, xmm1
	vminbf16 ymm5{k3}, ymm6, ymm5
	vminbf16 zmm2{k4}, zmm0, zword [0xdad]
	vminbf16 xmm7{k1}{z}, xmm5, xmm6
	vminbf16 ymm1{k3}{z}, ymm0, yword [0x23c]
	vminbf16 zmm4{k3}{z}, zmm3, zmm6
	vminbf16 xmm1, xmm2, oword [eax+1]
	vminbf16 xmm1, xmm7, oword [eax+64]
	vminbf16 ymm3, ymm2, yword [eax+1]
	vminbf16 ymm7, ymm5, yword [eax+64]
	vminbf16 zmm7, zmm2, zword [eax+1]
	vminbf16 zmm4, zmm6, zword [eax+64]
	vminbf16 xmm5, xmm0, [0x39f]
	vminbf16 ymm4, ymm3, [0x856]
	vminbf16 zmm4, zmm7, [0xab2]
