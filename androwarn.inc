ANDROWARN_PATH=${DIR}/tools/androwarn

function scan_androwarn_apk {
    debug "[${DATE}] Androwarn - Searching for security flaws... (androwarn.txt)"
    cd ${ANDROWARN_PATH}
    python androwarn.py -i ${APK_DIR}/apk/$FILE -r txt -v 3 -d >> $REPORT_DIR/androwarn.txt 2>&1
    cd $APK_DIR
}
