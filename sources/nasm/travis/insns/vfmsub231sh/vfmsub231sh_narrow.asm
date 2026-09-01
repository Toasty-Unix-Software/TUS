	vfmsub231sh xmm4, xmm0, word [0xd58]
	vfmsub231sh xmm4, word [0xd58]
	vfmsub231sh xmm5, xmm6, xmm0

%ifdef ERROR
	vfmsub231sh xmm9, xmm10, xmm9
	vfmsub231sh xmm28, xmm30, xmm28
%endif
