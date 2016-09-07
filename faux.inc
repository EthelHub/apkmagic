JADX_PATH=${DIR}/tools/jadx/bin

function create_project_structure {
    mkdir -p ${APK_DIR}/apk/src
    mkdir ${SOURCES_DIR}
    mkdir $LOG_DIR
    mkdir -p $REPORT_DIR/qark
    mkdir -p $REPORT_DIR/androbugs
    cp ${APK_DIR}/$FILE ${APK_DIR}/apk 
}

function extract_apk {
    DATE=`date +%x-%H:%M:%S`
    echo "[${DATE}] Extracting apk $FILE into ${APK_DIR}/apk/src... ($LOG_DIR/extract_apk.txt)"
    cd ${APK_DIR}/apk/src && unzip -q ${APK_DIR}/$FILE > $LOG_DIR/extract_apk.txt 2>&1
    cd ${APK_DIR}
}

function extract_sources {
    DATE=`date +%x-%H:%M:%S`
    echo "[${DATE}] Extracting sources into ${APK_DIR}/sources... (extract_sources.txt)"
    cd ${APK_DIR}
    ${JADX_PATH}/jadx apk/${FILE} -d sources --deobf > $LOG_DIR/extract_sources.txt 2>&1
}

function get_md5_apk {
    MD5=`md5 ${APK_DIR}/apk/$FILE | cut -d'=' -f 2 | cut -d' ' -f 2`
    echo "MD5 ($FILE) : $MD5" >> $REPORT_DIR/base_info.txt
}

function get_sha1_apk {
    SHA1=`shasum ${APK_DIR}/apk/$FILE | cut -d' ' -f 1`
    echo "SHA1 ($FILE) : $SHA1" >> $REPORT_DIR/base_info.txt
}
