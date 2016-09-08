WAPITI_PATH=${DIR}/tools/wapiti/bin
HTTP_SERVER_URL=127.0.0.1:8080

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
    cd $APK_DIR
}

function launch_https {
    DATE=`date +%x-%H:%M:%S`
    echo "[${DATE}] Phonegap - Launching webserver for self-exploitation..."
    cd $PHONEGAP_DIR
    http-server >> $LOG_DIR/http-server.log 2>&1 & 
    export PID_HTTP_SERVER=`echo $!`
    sleep 2
    launch_wapiti
    stop_https
}

function launch_wapiti {
    DATE=`date +%x-%H:%M:%S`
    echo "[${DATE}] Phonegap - Launching wapiti web scanner..."
    cd ${WAPITI_PATH} && python wapiti $HTTP_SERVER_URL -f txt -o $REPORT_DIR/wapiti.txt > $LOG_DIR/wapiti.txt 2>&1
    cd $APK_DIR
}

function stop_https {
    DATE=`date +%x-%H:%M:%S`
    echo "[${DATE}] Phonegap - Stopping web server..."
    kill -9 $PID_HTTP_SERVER > /dev/null 2>&1
}
