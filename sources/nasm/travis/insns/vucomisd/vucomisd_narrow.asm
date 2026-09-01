	vucomisd xmm2, xmm4
	vucomisd xmm7, qword [0x8a8]
	vucomisd xmm2, qword [0x76f]
	vucomisd xmm0, xmm7

%ifdef ERROR
	vucomisd xmm13, xmm9
	vucomisd xmm15, xmm8
	vucomisd xmm30, xmm18
	vucomisd xmm23, xmm27
%endif
