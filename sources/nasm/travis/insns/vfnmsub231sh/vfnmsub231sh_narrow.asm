	vfnmsub231sh xmm7, xmm6, word [0xc0e]
	vfnmsub231sh xmm7, word [0xc0e]
	vfnmsub231sh xmm3, xmm0, word [0x9d6]

%ifdef ERROR
	vfnmsub231sh xmm12, xmm9, xmm12
	vfnmsub231sh xmm22, xmm28, xmm20
%endif
