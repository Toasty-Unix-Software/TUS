	insertps xmm6, dword [0x39c], 0xce
	insertps xmm5, xmm0, 0x7d

%ifdef ERROR
	insertps xmm14, xmm14, 0x48
%endif
