	vfmsub231ss xmm7, xmm5, xmm0
	vfmsub231ss xmm0, xmm2, dword [0xfa2]
	vfmsub231ss xmm1, xmm3, xmm4
	vfmsub231ss xmm4, xmm2, dword [0xcb3]

%ifdef ERROR
	vfmsub231ss xmm10, xmm13, xmm13
	vfmsub231ss xmm14, xmm8, xmm10
	vfmsub231ss xmm20, xmm23, xmm20
	vfmsub231ss xmm17, xmm26, xmm29
%endif
