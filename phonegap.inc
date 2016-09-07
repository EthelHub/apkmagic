function detect_phonegap {
    if [ -d $APK_DIR/apk/src/assets/www ]; then
        DATE=`date +%x-%H:%M:%S`
        echo "[${DATE}] Phonegap/Apache Cordova web assets detected... (phonegap_web.txt)"
        export HAVE_PHONEGAP=1
        export PHONEGAP_DIR=$APK_DIR/apk/src/assets/www
        cd $PHONEGAP_DIR && find . > $REPORT_DIR/phonegap_web.txt
    fi
}

function parse_phonegap {
    if [ -d $PHONEGAP_DIR/js ]; then
        DATE=`date +%x-%H:%M:%S`
        echo "[${DATE}] Phonegap - Javascript sources detected... (phonegap_js.txt)"
        # Tratamiento especial Javascript
        find $PHONEGAP_DIR/js > $REPORT_DIR/phonegap_js.txt
    fi
    cd $DIR
}

function launch_https {
    DATE=`date +%x-%H:%M:%S`
    echo "[${DATE}] Phonegap - Launching webserver for self-exploitation..."
    cd $PHONEGAP_DIR
    PID_HTTP_SERVER=`http-server -s & echo $!`
    #TODO LANZAR SQLMAP CONTRA ID / USER / PASS
}

