# Silverberry programming language

## Try it out

### Build 

      cmake -B build
      cd build
      make -j8
      cat ../examples/factorial.sb | ./app/repl

You should see the evaluation of `3!` and a crash.
