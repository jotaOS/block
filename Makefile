export PROJNAME := block
export RESULT := block

.PHONY: all

all: $(RESULT)
	@

%: force
	@$(MAKE) -f $(STRIFE_HELPER)/Makefile $@ --no-print-directory
force: ;
