# Voltex Compiler

## Compilation

## Use Example

The following will produce a pdf from the output tex

```sh
./script/build.sh\
&& ./script/start.sh ./test/accept/01-two_component_series\
&& pdflatex output.tex\
&& xdg-open output.pdf
```

