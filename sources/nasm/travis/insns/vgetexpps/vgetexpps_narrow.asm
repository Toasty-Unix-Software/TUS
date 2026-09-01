	vgetexpps xmm0, xmm4
	vgetexpps xmm3, xmm1
	vgetexpps ymm3, yword [0xd51]
	vgetexpps ymm3, ymm5
	vgetexpps zmm5, zword [0xf49]
	vgetexpps zmm0, zmm6

%ifdef ERROR
	vgetexpps xmm12, xmm13
	vgetexpps ymm15, ymm13
	vgetexpps zmm11, zmm13
	vgetexpps xmm21, xmm24
	vgetexpps ymm28, ymm29
	vgetexpps zmm18, zmm28
%endif
