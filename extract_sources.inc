JADX_PATH=${DIR}/tools/jadx/bin
ENJARIFY_PATH=${DIR}/tools/enjarify
PROCYON_PATH=${DIR}/tools/procyon

function extract_sources_jadx {
    DATE=`date +%x-%H:%M:%S`
    debug "[${DATE}] Extracting sources using JadX into ${APK_DIR}/sources_jadx... (extract_sources_jadx.txt)"
    cd ${APK_DIR}
    ${JADX_PATH}/jadx apk/${FILE} -d sources --deobf > $LOG_DIR/extract_sources_jadx.txt 2>&1
}

function extract_sources_enjarify {
    DATE=`date +%x-%H:%M:%S`
    debug "[${DATE}] Reversing into .jar using Enjarify into ${APK_DIR}/sources_enjarify.jar... (extract_sources_enjarify.txt)"
    cd ${ENJARIFY_PATH}; ./enjarify.sh -o ${APK_DIR}/sources_enjarify.dex ${APK_DIR}/apk/${FILE} > $LOG_DIR/extract_sources_enjarify.txt 2>&1
    debug "[${DATE}] Extracting .jar using Procyon into ${APK_DIR}/sources_enjarify... (extract_sources_procyon.txt)"
    java -jar ${PROCYON_PATH}/decompiler.jar -jar ${APK_DIR}/sources_enjarify.dex -o  ${APK_DIR}/sources > $LOG_DIR/extract_sources_procyon.txt 2>&1
    rm -y  ${APK_DIR}/sources_enjarify.dex 2>/dev/null
}

