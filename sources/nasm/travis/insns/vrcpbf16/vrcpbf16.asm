default rel
	vrcpbf16 xmm5, xmm6
	vrcpbf16 xmm3, xmm5
	vrcpbf16 ymm4, yword [0xc86]
	vrcpbf16 ymm5, yword [0xbb4]
	vrcpbf16 zmm1, zmm3
	vrcpbf16 zmm3, zword [0x438]
	vrcpbf16 xmm15, xmm11
	vrcpbf16 ymm11, ymm12
	vrcpbf16 zmm8, zmm15
	vrcpbf16 xmm20, xmm24
	vrcpbf16 ymm25, ymm22
	vrcpbf16 zmm24, zmm16
	vrcpbf16 xmm6{k2}, xmm3
	vrcpbf16 ymm7{k5}, yword [0x7cd]
	vrcpbf16 zmm6{k4}, zword [0x9ce]
	vrcpbf16 xmm2{k1}{z}, oword [0x8d5]
	vrcpbf16 ymm3{k3}{z}, ymm0
	vrcpbf16 zmm2{k6}{z}, zmm1
	vrcpbf16 xmm1, oword [eax+1]
	vrcpbf16 xmm6, oword [eax+64]
	vrcpbf16 ymm4, yword [eax+1]
	vrcpbf16 ymm7, yword [eax+64]
	vrcpbf16 zmm1, zword [eax+1]
	vrcpbf16 zmm4, zword [eax+64]
	vrcpbf16 xmm3, [0xad0]
	vrcpbf16 ymm6, [0x92f]
	vrcpbf16 zmm6, [0x46e]
