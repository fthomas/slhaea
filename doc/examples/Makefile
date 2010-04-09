SOURCES  = $(wildcard *.cpp)
EXAMPLES = $(SOURCES:.cpp=)

all: $(EXAMPLES)

%: %.cpp
	g++ -I../.. -o "$@" "$<"

run: $(EXAMPLES)
	@for PROG in $^; do \
	  echo "Running $$PROG..."; \
	  ./$$PROG >/dev/null; \
	done

clean:
	rm -f $(EXAMPLES)

.PHONY: all run clean
