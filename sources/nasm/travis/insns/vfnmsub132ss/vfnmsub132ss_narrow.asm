	vfnmsub132ss xmm5, xmm5, xmm7
	vfnmsub132ss xmm6, xmm2, dword [0x3b7]
	vfnmsub132ss xmm5, xmm7, xmm3
	vfnmsub132ss xmm3, xmm6, dword [0xdb1]

%ifdef ERROR
	vfnmsub132ss xmm8, xmm13, xmm14
	vfnmsub132ss xmm8, xmm13, xmm8
	vfnmsub132ss xmm18, xmm17, xmm18
	vfnmsub132ss xmm30, xmm22, xmm23
%endif
