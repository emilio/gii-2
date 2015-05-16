SESSION_FOLDERS := $(wildcard session-*)
SESSION_PDFS := $(patsubst %, %/session.pdf, $(SESSION_FOLDERS))

all: $(SESSION_PDFS)
	echo $(SESSION_PDFS)

%/session.pdf:
	cat prelude.md > $(dir $@)session.md
	for f in $(dir $@)*.dia; do dia -e $$f.png $$f; done
	for f in $(dir $@)*.png; do echo "### $$f\\n![]($$f)" >> $(dir $@)session.md; done
	pandoc --from=markdown_github -S -s -V geometry:margin=1in -o $@ $(dir $@)session.md
	rm $(dir $@)*.dia.png
	rm $(dir $@)session.md

clean:
	rm $(SESSION_PDFS)
