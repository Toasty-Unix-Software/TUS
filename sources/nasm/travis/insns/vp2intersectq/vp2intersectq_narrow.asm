	vp2intersectq k3, xmm1, xmm5
	vp2intersectq k2, xmm2, oword [0x514]
	vp2intersectq k7, ymm4, ymm7
	vp2intersectq k5, ymm0, ymm6
	vp2intersectq k6, zmm5, zmm6
	vp2intersectq k3, zmm7, zword [0x5f2]

%ifdef ERROR
	vp2intersectq k6, xmm13, xmm9
	vp2intersectq k1, ymm13, ymm14
	vp2intersectq k5, zmm12, zmm13
	vp2intersectq k7, xmm18, xmm18
	vp2intersectq k1, ymm28, ymm30
	vp2intersectq k6, zmm31, zmm27
%endif
