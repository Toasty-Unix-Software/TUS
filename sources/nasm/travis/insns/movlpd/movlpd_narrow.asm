	movlpd qword [0x808], xmm4
	movlpd qword [0xa5a], xmm0
	movlpd xmm0, qword [0x841]
	movlpd xmm7, qword [0xa90]

%ifdef ERROR
	movlpd qword [0xa4c], xmm11
	movlpd xmm8, qword [0x19d]
%endif
