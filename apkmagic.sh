#!/bin/sh

# apkmagic v0.2
# EthelHub

# Dependencias
# jadx
# jshint
# http-server
# perl
# 
# Proximas
# https://manifestsecurity.com/appie/
# Androwarn
# https://manifestsecurity.com/android-application-security/

VERSION=0.2

if [ $# -eq 0 ]; then
    echo "apkmagic tool rev ${VERSION} - Android Security Scanner / Toolkit"
    echo "by EthelHub - Cybersercurity && Research"
    echo ""
    echo "Use : apkmagic FILENAME.APK"
    exit
fi 

# Paths
FILE=$1
APK_DIR=`pwd`
CUR_DIR=${PWD##*/}

# Includes
DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$DIR" ]]; then DIR="$PWD"; fi
. "$DIR/config.inc"    
. "$DIR/phonegap.inc"
. "$DIR/faux.inc"
. "$DIR/manifest.inc"
. "$DIR/strings.inc"
. "$DIR/code_analysis.inc"
. "$DIR/androbugs.inc"
. "$DIR/qark.inc"
. "$DIR/androwarn.inc"



# PROCESO PRINCIPAL

# Extracción y creación de directorios
echo "= Creating project directories and extracting sources..."
create_project_structure
extract_apk
extract_sources

# Parseo de Manifest
echo "= Extracting basic info and permissions..."
parse_name_version
parse_permissions
dangerous_permissions
parse_sdk_version

# Hashes de apk
get_md5_apk
get_sha1_apk

# Tratamiento PhoneGap / Cordova
detect_phonegap
if [ ${HAVE_PHONEGAP} ]; then
    # Tratamiento especial a www
    parse_phonegap
    scan_jshint
    search_multi $PHONEGAP_DIR phonegap_words.txt
    #launch_https
fi

echo "= Looking for debug / hardcoded variables..."
search_debug
search_multi $SOURCES_DIR sources_words.txt

echo "= Executing vulnerability scanners..."
scan_androbugs_apk
scan_androwarn_apk
scan_qark_apk

echo "= FINISHED."