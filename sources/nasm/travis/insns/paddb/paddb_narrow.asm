	paddb mm5, qword [0x841]
	paddb mm5, mm1
	paddb xmm5, xmm1
	paddb xmm1, xmm3

%ifdef ERROR
	paddb xmm8, xmm14
%endif
