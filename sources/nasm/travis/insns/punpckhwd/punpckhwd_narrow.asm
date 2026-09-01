	punpckhwd mm5, mm6
	punpckhwd mm0, mm3
	punpckhwd xmm7, oword [0x692]
	punpckhwd xmm4, xmm3

%ifdef ERROR
	punpckhwd xmm8, xmm12
%endif
