FILES := $(wildcard *.md)
TARGETS := $(patsubst %.md, %.html, $(FILES))
TARGETS := $(addprefix target/, $(TARGETS))

all: $(TARGETS)
	@echo > /dev/null

target/%.html: %.md
	@mkdir -p target
	cat $< | sed "s/^#/##/g" | pandoc --from=markdown_github --to=html5 -o $@

clean:
	rm -r target
