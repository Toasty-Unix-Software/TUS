	vfcmulcsh xmm2, xmm2, dword [0x34a]
	vfcmulcsh xmm2, dword [0x34a]
	vfcmulcsh xmm1, xmm0, xmm6

%ifdef ERROR
	vfcmulcsh xmm12, xmm10, xmm12
	vfcmulcsh xmm21, xmm23, xmm20
%endif
