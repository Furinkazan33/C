#! /bin/bash

# Verify that every functions defined in .h file is implemented in .c file

# regular expression definition of a function (without ending ";" or " {")
regexp_fun="^[a-z_]\{1,\}\s\{1,\}\*\{0,1\}[a-z_\*]*(.*)"

cat $1 | grep -e ${regexp_fun} | sed -e "s/;//g" | sort > result_h
cat $2 | grep -e ${regexp_fun} | sed -e "s/ {//g" | sort > result_c


diff result_h result_c

rc=$?

rm -f result_h result_c

exit $rc


