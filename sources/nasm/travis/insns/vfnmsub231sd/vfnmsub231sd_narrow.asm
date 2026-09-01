	vfnmsub231sd xmm4, xmm1, xmm4
	vfnmsub231sd xmm4, xmm5, qword [0x748]
	vfnmsub231sd xmm4, xmm1, xmm1
	vfnmsub231sd xmm2, xmm6, qword [0xaed]

%ifdef ERROR
	vfnmsub231sd xmm8, xmm15, xmm12
	vfnmsub231sd xmm12, xmm10, xmm10
	vfnmsub231sd xmm18, xmm16, xmm24
	vfnmsub231sd xmm29, xmm26, xmm22
%endif
