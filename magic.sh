#!/bin/sh

# apkmagic v0.1
# EthelHub

# Paths
FILE=$1
DIR=`pwd`
LOG_DIR=$DIR/logs
REPORT_DIR=$DIR/report
SOURCES_DIR=$DIR/sources
JADX_PATH=/Users/Kr0n0/Ing.Inversa/Utilidades/jadx/bin
ANDROID_SDK=/Users/Kr0n0/Library/Android/sdk

# Funciones auxiliares

function create_project_structure {
    mkdir -p apk/src
    mkdir $SOURCES_DIR
    mkdir $LOG_DIR
    mkdir -p $REPORT_DIR/qark
    cp $FILE apk 
}

function extract_apk {
    DATE=`date +%x-%H:%M:%S`
    echo "[${DATE}] Extracting apk $FILE into ${DIR}/apk/src... (extract_apk.txt)"
    cd $DIR
    cd apk/src && unzip -q ../$FILE > $LOG_DIR/extract_apk.txt && cd $DIR
}

function extract_sources {
    DATE=`date +%x-%H:%M:%S`
    echo "[${DATE}] Extracting sources into ${DIR}/sources... (extract_sources.txt)"
    cd $DIR
    PATH=$JADX_PATH:$PATH
    jadx apk/${FILE} -d sources --deobf > $LOG_DIR/extract_sources.txt 2>&1
}

function parse_permissions {
    DATE=`date +%x-%H:%M:%S`
    echo "[${DATE}] Parsing permissions from Manifest... (permissions.txt)"
    fgrep android.permission sources/AndroidManifest.xml | cut -d'"' -f 2 | sort > $REPORT_DIR/permissions.txt
    #TODO Parsear el resultado y mostrar cuales son peligrosas!
}

function detect_phonegap {
    if [ -d $DIR/apk/src/assets/www ]; then
        DATE=`date +%x-%H:%M:%S`
        echo "[${DATE}] Phonegap/Apache Cordova web assets detected... (phonegap_web.txt)"
        export HAVE_PHONEGAP=1
        export PHONEGAP_DIR=$DIR/apk/src/assets/www
        cd $PHONEGAP_DIR && find . > $REPORT_DIR/phonegap_web.txt
    fi
}

function parse_phonegap {
    if [ -d $PHONEGAP_DIR/js ]; then
        echo "[${DATE}] Phonegap - Javascript sources detected... (phonegap_js.txt)"
        # Tratamiento especial Javascript
        find $PHONEGAP_DIR/js > $REPORT_DIR/phonegap_js.txt
    fi
    cd $DIR
}

function search_multi {
    echo "[${DATE}] Search - Searching for interesting strings in $1... ($2.txt)"
    cd $1
    TEXTO=(user username password pass api key)
    for PALABRA in ${TEXTO[*]}
    do
        find . -type f -print0 ! -path '*.svn*' | xargs -0 grep --exclude '*.min.*' --exclude '*.css' $PALABRA - >> $REPORT_DIR/$2
    done
    cd $DIR
}

function search_debug {
    echo "[${DATE}] Search - Searching for debug initialization Strings.. (debug_words.txt)"
    cd $SOURCES_DIR
    find . -type f -print0 ! -path '*.svn*' | xargs -0 grep --exclude '*.min.*' --exclude '*.css' DEBUG | grep false | tr -s ' ' | cut -d':' -f 1 | sort >> $REPORT_DIR/debug_words.txt
}

function scan_qark_apk {
    echo "[${DATE}] Qark - Searching for security flaws... (qark/output.txt)"
    cd /Users/Kr0n0/Ing.Inversa/Utilidades/qark
    python qark.py --source 1 --pathtoapk $DIR/apk/$FILE --reportdir $REPORT_DIR/qark/ --exploit 0 --basesdk ${ANDROID_SDK} >> $REPORT_DIR/qark/output.txt
    cd $DIR
}

function scan_qark_sources {
    #TODO Test de que esto funciona!!
    echo "[${DATE}] Qark - Searching for security flaws... (qark/output.txt)"
    cd /Users/Kr0n0/Ing.Inversa/Utilidades/qark
    python qark.py --source 2 --manifest $SOURCES/AndroidManifest.xml --autodetectcodepath 1 --reportdir $REPORT_DIR/qark/ --exploit 0 --basesdk ${ANDROID_SDK} >> $REPORT_DIR/qark/output.txt
    cd $DIR
}

function scan_jshint {
    echo "[${DATE}] Phonegap - Searching for javascript code flaws... (jshint.txt)"
    cd $PHONEGAP_DIR
    find . -name "*.js" | xargs jshint - >> $REPORT_DIR/jshint.txt
    cd $DIR
}

function launch_https {
    echo "[${DATE}] Phonegap - Launching webserver for self-exploitation..."
    cd $PHONEGAP_DIR
    PID_HTTP_SERVER=`http-server -s & echo $!`
    #TODO LANZAR SQLMAP CONTRA ID / USER / PASS
}


# PROCESO PRINCIPAL
create_project_structure
extract_apk
extract_sources
parse_permissions
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
