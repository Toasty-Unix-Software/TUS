	vscalefsh xmm6, xmm3, xmm2
	vscalefsh xmm6, xmm2
	vscalefsh xmm6, xmm7, xmm4

%ifdef ERROR
	vscalefsh xmm11, xmm14, xmm9
	vscalefsh xmm20, xmm27, xmm27
%endif
