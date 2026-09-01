	vfnmsub123ss xmm3, xmm3, dword [0x1fe]
	vfnmsub123ss xmm3, xmm5, dword [0x74d]

%ifdef ERROR
	vfnmsub123ss xmm11, xmm14, xmm12
%endif
