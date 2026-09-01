	rdrand bx
	rdrand cx
	rdrand esi
	rdrand edi

%ifdef ERROR
	rdrand rcx
	rdrand rax
	rdrand r12w
	rdrand r10d
	rdrand r8
	rdrand r30w
	rdrand r24d
	rdrand r28
%endif
