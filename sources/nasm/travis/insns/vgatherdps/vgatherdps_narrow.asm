	vgatherdps xmm0, [xmm6*1], xmm5
	vgatherdps xmm4, [xmm7*1], xmm1
	vgatherdps ymm0, [ymm0*1], ymm5
	vgatherdps ymm1, [ymm4*1], ymm3
	vgatherdps xmm2, [xmm0*1]
	vgatherdps xmm0, [xmm5*1]
	vgatherdps ymm1, [ymm7*1]
	vgatherdps ymm0, [ymm3*1]

%ifdef ERROR
	vgatherdps xmm11, [xmm12*1], xmm10
	vgatherdps ymm14, [ymm12*1], ymm10
	vgatherdps xmm8, [xmm10*1]
	vgatherdps ymm11, [ymm13*1]
%endif
