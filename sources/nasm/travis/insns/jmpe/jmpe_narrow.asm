.L1:
	jmpe .L1
	jmpe .L1
	jmpe .L1
	jmpe .L1
	jmpe dx
	jmpe bp

%ifdef ERROR
	jmpe -14140948
	jmpe -88483359
	jmpe r13w
	jmpe r21w
%endif
