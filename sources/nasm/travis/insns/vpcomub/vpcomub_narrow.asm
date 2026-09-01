	vpcomub xmm5, xmm6, oword [0xd15], 0xc4
	vpcomub xmm5, oword [0xd15], 0xc4
	vpcomub xmm4, xmm3, xmm5, 0x78

%ifdef ERROR
	vpcomub xmm12, xmm14, xmm14, 0xc7
%endif
