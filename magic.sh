#!/bin/sh

# apkmagic v0.1
# EthelHub

# Dependencias
# jadx
# jshint
# http-server
# perl

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
. "$DIR/qark.inc"
. "$DIR/code_analysis.inc"


# PROCESO PRINCIPAL

# Extracción y creación de directorios
create_project_structure
extract_apk
extract_sources

# Parseo de Manifest
parse_name_version
parse_permissions
dangerous_permissions
parse_sdk_version

detect_phonegap
if [ ${HAVE_PHONEGAP} ]; then
    # Tratamiento especial a www
    parse_phonegap
    scan_jshint
    search_multi $PHONEGAP_DIR phonegap_words.txt
    #launch_https
fi
search_debug
search_multi $SOURCES_DIR sources_words.txt
scan_qark_apk
