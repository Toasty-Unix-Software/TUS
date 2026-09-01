	vminss xmm3, xmm1, dword [0xafc]
	vminss xmm3, dword [0xafc]
	vminss xmm1, xmm5, dword [0x5dd]
	vminss xmm7, xmm6, xmm7
	vminss xmm7, xmm7
	vminss xmm2, xmm1, xmm5

%ifdef ERROR
	vminss xmm10, xmm11, xmm11
	vminss xmm10, xmm12, xmm11
	vminss xmm27, xmm21, xmm18
	vminss xmm17, xmm17, xmm23
%endif
