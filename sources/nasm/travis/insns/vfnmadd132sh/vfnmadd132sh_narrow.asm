	vfnmadd132sh xmm3, xmm7, word [0x79a]
	vfnmadd132sh xmm3, word [0x79a]
	vfnmadd132sh xmm7, xmm5, word [0xf46]

%ifdef ERROR
	vfnmadd132sh xmm15, xmm13, xmm8
	vfnmadd132sh xmm30, xmm28, xmm30
%endif
