#! /bin/bash

#
# Diff between functions contained in .h and .c
#

[ $# -ne 2 ] && { echo "usage : $0 header_file c_file"; exit 1; }


# regular expression definition of a function (without ending ";" or " {")
regexp_fun="^[a-z_]\{1,\}\s\{1,\}\*\{0,1\}[a-z_\*]*(.*)"

cat $1 | grep -e ${regexp_fun} | sed -e "s/;//g" | sort > result_h
cat $2 | grep -e ${regexp_fun} | sed -e "s/ {//g" | sort > result_c

# Result as two columns
diff -y --suppress-common-lines -W150 result_h result_c
rc=$?

rm -f result_h result_c

exit $rc


