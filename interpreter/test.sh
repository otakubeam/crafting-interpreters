#! /bin/sh

######################################################################

black=0; red=1; green=2; yellow=3; blue=4; pink=5; cyan=6; white=7;
bold=$(tput bold); normal=$(tput sgr0);

cecho () {
  local _color=$1; shift
  echo -e "$(tput setaf $_color)$@$(tput sgr0)"
}

bold() {
  local _color=$1; 
  echo -e ${bold}$(cecho $_color $2)${normal}
}

######################################################################

runtest() {
  local _testname=$1
  # https://stackoverflow.com/questions/34964332
  $(timeout 0.3 ./examples/repl $_testname &> /dev/null)
}

collect_debug_artifacts() {
  local _testname=$(basename -s .sb $1)
  local _dirname="artifacts/$_testname"
  bold $blue "Debug artifacts in '$(bold $white $_dirname)'\n"
  mkdir -p "$_dirname/dots"
  (
    local _base=$(pwd)
    cd $_dirname
    $(timeout 0.1 $_base/examples/repl $_base/$1 --debug &> log)
  )
  cp graph.sh $_dirname
}

######################################################################

report_test() {
  local f=$1
  local name=$(cecho $white "[!] Running test $(basename $f)")
  printf '%-60s ... ' "$name"

  if runtest $f; then 
    printf $(bold $green "OK!") && echo
  else
    printf $(bold $red "FAIL!") && echo
    collect_debug_artifacts $f
  fi
}

######################################################################

tests=$(find . -regextype "egrep" -iregex ".*-test-.*sb" | sort)

for f in $tests; do
  report_test $f
done

bold $green "-----------------------------------------------------------"
bold $green "All tests passed! (ideally)"

######################################################################
