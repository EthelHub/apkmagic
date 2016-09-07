function scan_jshint {
    DATE=`date +%x-%H:%M:%S`
    echo "[${DATE}] Phonegap - Searching for javascript code flaws... (jshint.txt)"
    cd $PHONEGAP_DIR
    find . -print0 -type f -name "*.js" | xargs -0 jshint - >> $REPORT_DIR/jshint.txt
    cd ${APK_DIR}
}
