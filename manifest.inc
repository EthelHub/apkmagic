MANIFEST_FILE=${SOURCES_DIR}/AndroidManifest.xml
PERMISSIONS_LIST=$DIR/permissions.list

function parse_permissions {
    DATE=`date +%x-%H:%M:%S`
    debug "[${DATE}] Parsing permissions from Manifest... (permissions.txt)"
    fgrep android.permission $MANIFEST_FILE | cut -d'"' -f 2 | sort > $REPORT_DIR/permissions.txt
}

function dangerous_permissions {
    DATE=`date +%x-%H:%M:%S`
    debug "[${DATE}] Looking for dangerous permissions... (dangerous.txt)"
    for PERMISO in `cat $PERMISSIONS_LIST`
    do 
        ENCONTRADO=`grep -c $PERMISO $MANIFEST_FILE`
        if [ $ENCONTRADO -ne 0 ]; then
            echo "Permiso peligroso detectado : $PERMISO" >> $REPORT_DIR/dangerous.txt
        fi
    done
}

function parse_name_version {
    PACKAGE_NAME=`perl -e 'while($line=<>) { if ($line=~ /package\s*=\s*"([^"]+)"/) { print "$1\n";}}' < $SOURCES_DIR/AndroidManifest.xml`
    VERSION_NAME=`perl -e 'while($line=<>) { if ($line=~ /versionName\s*=\s*"([^"]+)"/) { print "$1\n";}}' < $SOURCES_DIR/AndroidManifest.xml`
    echo "Package : ${PACKAGE_NAME}" >> $REPORT_DIR/base_info.txt
    echo "Version : ${VERSION_NAME}" >> $REPORT_DIR/base_info.txt
}

function parse_sdk_version {
    MIN_SDK_VERSION=`perl -e 'while($line=<>) { if ($line=~ /minSdkVersion\s*=\s*"([^"]+)"/) { print "$1\n";}}' < $SOURCES_DIR/AndroidManifest.xml`
    TARGET_SDK_VERSION=`perl -e 'while($line=<>) { if ($line=~ /targetSdkVersion\s*=\s*"([^"]+)"/) { print "$1\n";}}' < $SOURCES_DIR/AndroidManifest.xml`
    echo "Minimum SDK Version : ${MIN_SDK_VERSION}" >> $REPORT_DIR/base_info.txt
    echo "Target SDK Version : ${TARGET_SDK_VERSION}" >> $REPORT_DIR/base_info.txt
}
