	vpcomud xmm7, xmm4, oword [0x7a4], 0x62
	vpcomud xmm7, oword [0x7a4], 0x62
	vpcomud xmm0, xmm7, xmm5, 0xac

%ifdef ERROR
	vpcomud xmm10, xmm9, xmm12, 0xb2
%endif
