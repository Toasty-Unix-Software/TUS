	vp2intersectd k3, xmm4, xmm2
	vp2intersectd k7, xmm5, xmm7
	vp2intersectd k3, ymm5, yword [0xb6d]
	vp2intersectd k7, ymm6, ymm0
	vp2intersectd k3, zmm5, zmm6
	vp2intersectd k5, zmm4, zmm5

%ifdef ERROR
	vp2intersectd k7, xmm9, xmm8
	vp2intersectd k2, ymm13, ymm8
	vp2intersectd k4, zmm10, zmm13
	vp2intersectd k4, xmm29, xmm25
	vp2intersectd k4, ymm31, ymm29
	vp2intersectd k3, zmm18, zmm21
%endif
