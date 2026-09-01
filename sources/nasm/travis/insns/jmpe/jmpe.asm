default rel
.L1:
	jmpe .L1
	jmpe .L1
	jmpe .L1
	jmpe .L1
	jmpe -14140948
	jmpe -88483359
	jmpe r13w
	jmpe r21w
	jmpe qword [eax+1]
	jmpe qword [eax+64]
	jmpe qword [eax+1]
	jmpe qword [eax+64]
	jmpe [0xe87]
	jmpe [0x97a]

%ifdef ERROR
	jmpe dx
	jmpe bp
%endif
