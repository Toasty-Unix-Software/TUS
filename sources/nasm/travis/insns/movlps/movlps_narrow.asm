	movlps xmm3, qword [0x816]
	movlps xmm3, qword [0xf00]
	movlps qword [0x5f8], xmm3
	movlps qword [0x54f], xmm7

%ifdef ERROR
	movlps xmm14, qword [0x602]
	movlps qword [0x49e], xmm14
%endif
