QARK_PATH=${DIR}/tools/qark

function scan_qark_apk {
    DATE=`date +%x-%H:%M:%S`
    echo "[${DATE}] Qark - Searching for security flaws... (qark/output.txt)"
    cd ${QARK_PATH}
    python qark.py --source 1 --pathtoapk ${APK_DIR}/apk/$FILE --reportdir $REPORT_DIR/qark/ --exploit 0 --install 0 --basesdk ${ANDROID_SDK} >> $REPORT_DIR/qark/output.txt
    cd $APK_DIR
}

function scan_qark_sources {
    #TODO Test de que esto funciona!!
    DATE=`date +%x-%H:%M:%S`
    echo "[${DATE}] Qark - Searching for security flaws... (qark/output.txt)"
    cd ${QARK_PATH}
    python qark.py --source 2 --manifest $SOURCES/AndroidManifest.xml --autodetectcodepath 1 --reportdir $REPORT_DIR/qark/ --exploit 0 --install 0 --basesdk ${ANDROID_SDK} >> $REPORT_DIR/qark/output.txt
    cd $APK_DIR
}

