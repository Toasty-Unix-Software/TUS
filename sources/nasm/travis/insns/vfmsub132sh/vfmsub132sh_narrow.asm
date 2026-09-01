	vfmsub132sh xmm1, xmm5, word [0x7e6]
	vfmsub132sh xmm1, word [0x7e6]
	vfmsub132sh xmm6, xmm6, word [0x9d0]

%ifdef ERROR
	vfmsub132sh xmm8, xmm11, xmm10
	vfmsub132sh xmm25, xmm30, xmm25
%endif
