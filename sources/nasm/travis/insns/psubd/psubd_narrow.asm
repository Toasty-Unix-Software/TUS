	psubd mm3, qword [0x8ff]
	psubd mm0, qword [0xa99]
	psubd xmm6, xmm7
	psubd xmm3, oword [0x5cd]

%ifdef ERROR
	psubd xmm13, xmm9
%endif
