	vrcpsh xmm6, xmm6, xmm3
	vrcpsh xmm6, xmm3
	vrcpsh xmm5, xmm3, xmm5

%ifdef ERROR
	vrcpsh xmm12, xmm12, xmm10
	vrcpsh xmm26, xmm27, xmm22
%endif
