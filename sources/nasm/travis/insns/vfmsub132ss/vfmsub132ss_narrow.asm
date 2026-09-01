	vfmsub132ss xmm6, xmm3, xmm2
	vfmsub132ss xmm4, xmm3, xmm3
	vfmsub132ss xmm2, xmm7, dword [0x686]
	vfmsub132ss xmm2, xmm6, xmm2

%ifdef ERROR
	vfmsub132ss xmm10, xmm13, xmm12
	vfmsub132ss xmm13, xmm15, xmm9
	vfmsub132ss xmm19, xmm18, xmm29
	vfmsub132ss xmm29, xmm25, xmm16
%endif
