	vfnmsub132sd xmm5, xmm2, xmm5
	vfnmsub132sd xmm5, xmm2, xmm5
	vfnmsub132sd xmm4, xmm7, xmm7
	vfnmsub132sd xmm6, xmm5, qword [0x8b0]

%ifdef ERROR
	vfnmsub132sd xmm12, xmm8, xmm9
	vfnmsub132sd xmm12, xmm11, xmm8
	vfnmsub132sd xmm24, xmm31, xmm26
	vfnmsub132sd xmm21, xmm22, xmm29
%endif
