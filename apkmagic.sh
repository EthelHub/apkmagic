#!/bin/sh

# apkmagic
# Android Automated Security Scanner / Toolkit
# 
# by EthelHub - Cybersercurity && Research

# Dependencies
# jadx
# jshint
# http-server
# perl
# nikto

VERSION=0.6

# Paths
APK_DIR=`pwd`
CUR_DIR=${PWD##*/}
DEBUG=0

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
. "$DIR/report.inc"

# PROCESO PRINCIPAL
echo "apkmagic tool rev ${VERSION} - Android Security Scanner / Toolkit"
echo "by EthelHub - Cybersercurity && Research"
parameters $@
echo ""
echo "Package : $FILE"
echo ""

# Extracción y creación de directorios
echo "= Creating project directories and extracting sources..."
create_project_structure
extract_apk
extract_sources

# Parseo de Manifest
echo "" && echo "= Extracting package basic info and manifest permissions..."
parse_name_version
parse_permissions
dangerous_permissions
parse_sdk_version

# Hashes de apk
get_md5_apk
get_sha1_apk

echo "" && echo "= Looking for PhoneGap/Cordova Framework..."
# Tratamiento PhoneGap / Cordova
detect_phonegap
if [ ${HAVE_PHONEGAP} ]; then
    # Tratamiento especial a www
    parse_phonegap
    scan_jshint
    search_multi $PHONEGAP_DIR phonegap_words.txt
    launch_https
fi

echo "" && echo "= Looking for hardcoded interesting strings..."
search_debug
search_multi $SOURCES_DIR sources_words.txt
search_sql
search_emails
search_urls
search_api

echo "" && echo "= Executing Android vulnerability scanners..."
scan_androbugs_apk
scan_androwarn_apk
scan_qark_apk

echo "" && echo "= Generating report..."
report

echo "" && echo "= FINISHED."
