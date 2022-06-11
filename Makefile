export PROJNAME := block
export RESULT := block

.PHONY: all

all: $(RESULT)
	@

%: force
	@$(MAKE) -f ../helper/Makefile $@ --no-print-directory
force: ;
