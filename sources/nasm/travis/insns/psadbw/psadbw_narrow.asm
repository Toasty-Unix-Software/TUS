	psadbw mm6, mm3
	psadbw mm5, mm1
	psadbw xmm4, oword [0x275]
	psadbw xmm4, xmm3

%ifdef ERROR
	psadbw xmm15, xmm12
%endif
