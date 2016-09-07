ANDROBUGS_PATH=$DIR/tools/androbugs

function scan_androbugs_apk {
    DATE=`date +%x-%H:%M:%S`
    echo "[${DATE}] Androbugs - Searching for security flaws... (androbugs/output.txt)"
    cd $ANDROBUGS_PATH 
    python androbugs.py -f ${APK_DIR}/apk/$FILE -m single -o $REPORT_DIR/androbugs/  >> $REPORT_DIR/androbugs/output.txt
    cd $APK_DIR
}
