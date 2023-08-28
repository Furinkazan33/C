#! /bin/bash

#
# Copy dependencies ${2-} given into project folder ${1}/core/{include, lib, obj}
#

[ "$#" -lt 2 ] && { 
	echo "usage : $0 project_folder dep[1..n]";
	exit 1; 
}

DIR_MODULES=~/repos/C/modules
DIR_PROJECT=$1
shift

function exif_not_folder() {
	[ -d "${1}" ] || {
		echo "${1} folder not found"; 
		exit 1; 
	}
}

# usage: dep_copy file destination_folder
function dep_copy() {
	file=$(find "${DIR_MODULES}" -type f -name "${1}")

	echo "Found: ${file}"

	n=$(echo ${file} | wc -w)

	[ ${n} -gt 1 ] && { echo "Too many files found"; exit 1; }

	cp ${file} ${2}/
}

function main() {
	exif_not_folder "${DIR_PROJECT}/core"
	exif_not_folder "${DIR_PROJECT}/core/include"
	exif_not_folder "${DIR_PROJECT}/core/obj"
	exif_not_folder "${DIR_PROJECT}/core/lib"

	for d in $@; do
		dep_copy "${d}.h" "${DIR_PROJECT}/core/include/"
		dep_copy "${d}.o" "${DIR_PROJECT}/core/obj/"
		dep_copy "lib${d}.a" "${DIR_PROJECT}/core/lib/"
	done
}



main "${@}"



