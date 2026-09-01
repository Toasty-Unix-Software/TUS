	pmulhrsw mm0, mm7
	pmulhrsw mm3, qword [0xfb5]
	pmulhrsw xmm7, xmm4
	pmulhrsw xmm7, xmm2

%ifdef ERROR
	pmulhrsw xmm10, xmm10
%endif
