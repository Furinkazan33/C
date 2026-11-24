#! /bin/bash

#
# Diff between functions contained in .h and .c
#

[ $# -ne 2 ] && { echo "usage : $0 header_file c_file"; exit 1; }


# function type and name till (
re_0_m_spaces="\s\{0,\}"
re_1_m_spaces="\s\{1,\}"
re_ident="[a-z_]\{1,\}"
re_0_3_stars="\*\{0,3\}"
re_fun="^${re_ident}${re_1_m_spaces}${re_0_3_stars}${re_ident}${re_0_m_spaces}("

cat $1 | grep -e ${re_fun} | sed -e "s/(.*//g" | sort > result_h
cat $2 | grep -e ${re_fun} | sed -e "s/(.*//g" | sort > result_c

# Result as two columns
diff -y --suppress-common-lines -W150 result_h result_c
rc=$?

rm -f result_h result_c

exit $rc


