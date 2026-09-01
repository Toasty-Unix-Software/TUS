	vfnmsub213ss xmm3, xmm6, xmm7
	vfnmsub213ss xmm5, xmm0, dword [0x10e]
	vfnmsub213ss xmm7, xmm7, xmm6
	vfnmsub213ss xmm5, xmm3, xmm4

%ifdef ERROR
	vfnmsub213ss xmm11, xmm8, xmm9
	vfnmsub213ss xmm14, xmm8, xmm14
	vfnmsub213ss xmm24, xmm16, xmm19
	vfnmsub213ss xmm31, xmm18, xmm22
%endif
