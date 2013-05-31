#!/bin/sh

FAUST_DIR=/Applications/faust-0.9.58
AU_DIR=/Users/reza/Library/Audio/Plug-Ins/Components

FAUST_AU_DIR=$FAUST_DIR/architecture/AU/
cd $FAUST_AU_DIR

if [ $# -lt 1 ]
then
	echo faust dsp file not specified
	exit
fi


faust -a $FAUST_DIR/architecture/au.cpp $1 -o Source/au-output.cpp

FULL_FILE_NAME=$1

FILE_NAME=$(BASENAME "$FULL_FILE_NAME")
EXTENSION="${FILE_NAME##*.}"
FILE_NAME="${FILE_NAME%.*}"



if [ $# -lt 2 ]
then
	MANUFACTURER="Grame"
else
	MANUFACTURER=$2
fi


NAME=$MANUFACTURER:' '"$FILE_NAME"

if [ $# -lt 3 ]
then
	SUB_TYPE=${FILE_NAME:0:4}
else
	SUB_TYPE=$3
fi

if [ $# -lt 4 ]
then
	MANF=${MANUFACTURER:0:4}
else
	MANF=$4
fi

perl -pi -e 's/_NAME_/'"$NAME"'/g' Info.plist
perl -pi -e 's/_DESC_/'"$NAME"'/g' Info.plist
perl -pi -e 's/_STYP_/'"$SUB_TYPE"'/g'  Info.plist
perl -pi -e 's/_MANF_/'"$MANF"'/g' Info.plist 
perl -pi -e 's/_BUNDLE_ID_/'"$FILE_NAME"'/g' Info.plist 

perl -pi -e 's/_BUNDLE_NAME_/'"$FILE_NAME"'/g' English.lproj/InfoPlist.strings 
perl -pi -e 's/_BUNDLE_INFO_/'"$NAME"'/g' English.lproj/InfoPlist.strings 

perl -pi -e 's/_NAME_/'"$NAME"'/g' Source/AUSource/FaustAU.r
perl -pi -e 's/_DESC_/'"$NAME"'/g' Source/AUSource/FaustAU.r 

perl -pi -e 's/_STYP_/'"$SUB_TYPE"'/g' Source/AUSource/FaustAUVersion.h
perl -pi -e 's/_MANF_/'"$MANF"'/g' Source/AUSource/FaustAUVersion.h 

xcodebuild CONFIGURATION_BUILD_DIR=/Users/reza/Library/Audio/Plug-Ins/Components

rm Info.plist
cp Info.plisttemplate Info.plist

rm English.lproj/InfoPlist.strings
cp English.lproj/InfoPlist.stringstemplate English.lproj/InfoPlist.strings

rm Source/AUSource/FaustAU.r
cp Source/AUSource/FaustAU.rtemplate Source/AUSource/FaustAU.r

rm Source/AUSource/FaustAUVersion.h
cp Source/AUSource/FaustAUVersion.htemplate Source/AUSource/FaustAUVersion.h

cp -r $AU_DIR/FaustAU.component $AU_DIR/"$FILE_NAME".component/
rm -r $AU_DIR/FaustAU.component

