	vfmadd132sh xmm1, xmm6, word [0x201]
	vfmadd132sh xmm1, word [0x201]
	vfmadd132sh xmm3, xmm0, word [0x7fa]

%ifdef ERROR
	vfmadd132sh xmm10, xmm9, xmm9
	vfmadd132sh xmm24, xmm27, xmm16
%endif
