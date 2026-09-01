	rdmsr
	rdmsr

%ifdef ERROR
	rdmsr rdi, 0x36e212fc
	rdmsr rbx, 0x3ad6bf64
	rdmsr r12, 0x23ea4b53
	rdmsr r30, 0x25384190
%endif
