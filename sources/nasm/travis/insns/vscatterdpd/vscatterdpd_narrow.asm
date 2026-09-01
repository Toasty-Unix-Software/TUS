	vscatterdpd [xmm5*1], xmm5
	vscatterdpd [xmm2*1], xmm5
	vscatterdpd [xmm0*1], ymm7
	vscatterdpd [xmm1*1], ymm3
	vscatterdpd [ymm3*1], zmm7
	vscatterdpd [ymm7*1], zmm5

%ifdef ERROR
	vscatterdpd [xmm13*1], xmm15
	vscatterdpd [xmm9*1], ymm10
	vscatterdpd [ymm11*1], zmm13
	vscatterdpd [xmm25*1], xmm28
	vscatterdpd [xmm25*1], ymm18
	vscatterdpd [ymm29*1], zmm27
%endif
