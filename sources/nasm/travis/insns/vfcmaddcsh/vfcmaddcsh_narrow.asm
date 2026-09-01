	vfcmaddcsh xmm2, xmm3, dword [0x392]
	vfcmaddcsh xmm2, dword [0x392]
	vfcmaddcsh xmm0, xmm6, xmm0

%ifdef ERROR
	vfcmaddcsh xmm10, xmm9, xmm11
	vfcmaddcsh xmm16, xmm21, xmm20
%endif
