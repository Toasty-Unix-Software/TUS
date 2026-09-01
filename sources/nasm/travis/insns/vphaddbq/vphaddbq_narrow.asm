	vphaddbq xmm5, oword [0xbf9]
	vphaddbq xmm5
	vphaddbq xmm5, oword [0xaff]

%ifdef ERROR
	vphaddbq xmm8, xmm8
%endif
