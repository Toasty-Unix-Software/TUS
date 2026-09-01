	vlddqu xmm7, oword [0x149]
	vlddqu xmm1, oword [0xade]
	vlddqu ymm7, yword [0x66d]
	vlddqu ymm7, yword [0x966]

%ifdef ERROR
	vlddqu xmm12, oword [0x827]
	vlddqu ymm8, yword [0x488]
%endif
