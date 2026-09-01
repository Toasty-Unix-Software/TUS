	cvtdq2pd xmm6, oword [0x308]
	cvtdq2pd xmm5, oword [0xe40]

%ifdef ERROR
	cvtdq2pd xmm10, xmm9
%endif
