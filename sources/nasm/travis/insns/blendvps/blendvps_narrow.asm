	blendvps xmm1, xmm6, xmm0
	blendvps xmm7, xmm6, xmm0
	blendvps xmm1, xmm1
	blendvps xmm6, xmm0

%ifdef ERROR
	blendvps xmm12, xmm10, xmm0
	blendvps xmm14, xmm15
%endif
