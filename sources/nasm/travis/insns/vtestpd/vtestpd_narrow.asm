	vtestpd xmm5, xmm4
	vtestpd xmm1, xmm0
	vtestpd ymm3, yword [0xcc7]
	vtestpd ymm3, ymm6

%ifdef ERROR
	vtestpd xmm11, xmm8
	vtestpd ymm14, ymm11
%endif
