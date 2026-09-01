default rel
	vp2intersectd k3, xmm4, xmm2
	vp2intersectd k7, xmm5, xmm7
	vp2intersectd k3, ymm5, yword [0xb6d]
	vp2intersectd k7, ymm6, ymm0
	vp2intersectd k3, zmm5, zmm6
	vp2intersectd k5, zmm4, zmm5
	vp2intersectd k7, xmm9, xmm8
	vp2intersectd k2, ymm13, ymm8
	vp2intersectd k4, zmm10, zmm13
	vp2intersectd k4, xmm29, xmm25
	vp2intersectd k4, ymm31, ymm29
	vp2intersectd k3, zmm18, zmm21
	vp2intersectd k3, xmm5, oword [eax+1]
	vp2intersectd k1, xmm6, oword [eax+64]
	vp2intersectd k7, ymm3, yword [eax+1]
	vp2intersectd k2, ymm7, yword [eax+64]
	vp2intersectd k7, zmm6, zword [eax+1]
	vp2intersectd k3, zmm3, zword [eax+64]
	vp2intersectd k7, xmm4, [0x7b7]
	vp2intersectd k6, ymm2, [0x2b8]
	vp2intersectd k3, zmm2, [0x741]
