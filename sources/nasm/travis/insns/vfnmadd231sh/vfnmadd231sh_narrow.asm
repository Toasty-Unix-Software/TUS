	vfnmadd231sh xmm1, xmm5, word [0xeb5]
	vfnmadd231sh xmm1, word [0xeb5]
	vfnmadd231sh xmm3, xmm5, xmm0

%ifdef ERROR
	vfnmadd231sh xmm8, xmm10, xmm15
	vfnmadd231sh xmm24, xmm22, xmm28
%endif
