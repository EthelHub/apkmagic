JADX_PATH=${DIR}/tools/jadx/bin

function display_help {
  echo "Use : apkmagic -f FILENAME.APK [extra_options]"
  echo "Extra options:"
  echo ""
  echo "-h / --help : Show this help."
  echo "-v / --verbose : Verbose / Debug mode."
  echo ""
}

function parameters {
    if [ $# -eq 0 ]; then
        display_help
        exit 0
    fi
    while :
    do
        case "$1" in
            -f | --file)
                FILE="$2"   # You may want to check validity of $2
                if [ $FILE == "" ]; then
                    display_help
                    exit -1
                fi
                shift 2
            ;;
            -h | --help)
                display_help  # Call your function
                # no shifting needed here, we're done.
                exit 0
            ;;
            -v | --verbose)
                DEBUG=1
                shift
            ;;
            --) # End of all options
                shift
                break;
            ;;
            -*)
                echo "Error: Unknown option: $1" >&2
                exit 1
            ;;
            *)  # No more options
                break
            ;;
    esac
done
}


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
    debug "[${DATE}] Extracting apk $FILE into ${APK_DIR}/apk/src... (extract_apk.txt)"
    cd ${APK_DIR}/apk/src && unzip -q ${APK_DIR}/$FILE > $LOG_DIR/extract_apk.txt 2>&1
    cd ${APK_DIR}
}

function extract_sources {
    DATE=`date +%x-%H:%M:%S`
    debug "[${DATE}] Extracting sources into ${APK_DIR}/sources... (extract_sources.txt)"
    cd ${APK_DIR}
    ${JADX_PATH}/jadx apk/${FILE} -d sources --deobf > $LOG_DIR/extract_sources.txt 2>&1
}

function get_md5_apk {
    if [ $OSTYPE == "linux" ]; then
        MD5=`md5sum ${APK_DIR}/apk/$FILE | cut -d' ' -f 1`
    else
        MD5=`md5 ${APK_DIR}/apk/$FILE | cut -d'=' -f 2 | cut -d' ' -f 2`
    fi
    echo "MD5 ($FILE) : $MD5" >> $REPORT_DIR/base_info.txt
}

function get_sha1_apk {
    SHA1=`shasum ${APK_DIR}/apk/$FILE | cut -d' ' -f 1`
    echo "SHA1 ($FILE) : $SHA1" >> $REPORT_DIR/base_info.txt
}

function debug {
    if [ ${DEBUG} -eq 1 ]; then
        echo "$1"
    fi
}