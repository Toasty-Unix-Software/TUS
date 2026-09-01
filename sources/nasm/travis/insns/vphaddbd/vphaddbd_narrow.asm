	vphaddbd xmm0, oword [0xc66]
	vphaddbd xmm0
	vphaddbd xmm5, xmm3

%ifdef ERROR
	vphaddbd xmm10, xmm12
%endif
