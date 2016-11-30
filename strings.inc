function search_multi {
    DATE=`date +%x-%H:%M:%S`
    debug "[${DATE}] Search - Searching for interesting strings in $1... ($2)"
    cd $1
    TEXTO=(user username password pass api key http login webservice)
    for PALABRA in ${TEXTO[*]}
    do
        find . -type f -print0 ! -path '*.svn*' | xargs -0 grep -i  --exclude '*.min.*' --exclude '*.css' $PALABRA -  \
            | grep -v "com/google/" \
            | grep -v "android/support" \
            | grep -v "org/apache" \
            | grep -v "AndroidManifest.xml" \
            | grep -v "Binary file" \
            | grep -v "jadx" \
            >> $REPORT_DIR/$2
    done
    VSTRINGS_FOUND=`cat $REPORT_DIR/$2 | sort | uniq | wc -l`
    echo "Interesting strings found in $1 : ${VSTRINGS_FOUND}"
    cd $APK_DIR
}

function search_debug {
    DATE=`date +%x-%H:%M:%S`
    debug "[${DATE}] Search - Searching for debug initialization Strings.. (debug_words.txt)"
    cd $SOURCES_DIR
    find . -type f -print0 ! -path '*.svn*' | xargs -0 grep -i --exclude '*.min.*' --exclude '*.css' DEBUG | grep false | tr -s ' ' | cut -d':' -f 1 | sort >> $REPORT_DIR/debug_words.txt
    DEBUG_FOUND=`cat $REPORT_DIR/debug_words.txt | wc -l`
    echo "Debug initialization strings found : ${DEBUG_FOUND}"
    cd $APK_DIR
}

function search_sql {
    DATE=`date +%x-%H:%M:%S`
    debug "[${DATE}] Search - Searching for hardcoded SQL CRUD strings.. (sql_words.txt)"
    cd $SOURCES_DIR
    declare -a TEXTO=("SELECT " "UPDATE " "INSERT " "DROP ")
    for PALABRA in ${TEXTO[@]}
    do
        find . -type f -print0 ! -path '*.svn' ! -path '*.git' | xargs -0 grep --exclude '*.min.*' --exclude '*.css' $PALABRA - | grep "${PALABRA} ">> $REPORT_DIR/sql_words.txt
    done
    SQL_FOUND=`cat $REPORT_DIR/sql_words.txt | wc -l`
    echo "SQL CRUD strings found : ${SQL_FOUND}"
    cd $APK_DIR
}

function search_emails {
    DATE=`date +%x-%H:%M:%S`
    debug "[${DATE}] Search - Searching for hardcoded email accounts... (email_words.txt)"
    cd $SOURCES_DIR
    find . -type f -print0 ! -path '*.svn*' | xargs -0 grep -h -E -o "\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,6}\b" >> $REPORT_DIR/email_words.txt.t1
    cd $APK_DIR/apk/src
    find . -type f -print0 ! -path '*.svn*' | xargs -0 grep -h -E -o "\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,6}\b" >> $REPORT_DIR/email_words.txt.t2
    cat $REPORT_DIR/email_words.txt.t1 $REPORT_DIR/email_words.txt.t2 | tr -s ' ' | cut -d':' -f 2 | sort | uniq > $REPORT_DIR/email_words.txt 2>/dev/null
    rm $REPORT_DIR/email_words.txt.t1 $REPORT_DIR/email_words.txt.t2
    EMAIL_FOUND=`cat $REPORT_DIR/email_words.txt | wc -l`
    echo "E-Mail addresses found : ${EMAIL_FOUND}"
    cd $APK_DIR
}

function search_urls {
    DATE=`date +%x-%H:%M:%S`
    debug "[${DATE}] Search - Searching for hardcoded URLs... (urls_words.txt)"
    cd $SOURCES_DIR
    find . -type f -print0 ! -path '*.svn*' | xargs -0 egrep -h -o 'https?://[^ ]+' | grep -v "Binary file" | grep -v "schema.org" >> $REPORT_DIR/urls_words.txt.t1
    cd $APK_DIR/apk/src
    find . -type f -print0 ! -path '*.svn*' | xargs -0 egrep -h -o 'https?://[^ ]+' | grep -v "Binary file" | grep -v "schema.org" >> $REPORT_DIR/urls_words.txt.t2
    cat $REPORT_DIR/urls_words.txt.t1 $REPORT_DIR/urls_words.txt.t2 | sort | uniq > $REPORT_DIR/urls_words.txt
    rm $REPORT_DIR/urls_words.txt.t1 $REPORT_DIR/urls_words.txt.t2
}

function search_api {
    DATE=`date +%x-%H:%M:%S`
    debug "[${DATE}] Search - Searching for remote API access URLs... (api_words.txt)"
    cat $REPORT_DIR/urls_words.txt | grep -i api >> $REPORT_DIR/api_words.txt
    cat $REPORT_DIR/api_words.txt | cut -d'/' -f 3 | cut -d':' -f 1 | sort | uniq > $REPORT_DIR/api_resume.txt
    API_FOUND=`cat $REPORT_DIR/api_resume.txt | wc -l`
    echo "Remote API access URLs found : ${API_FOUND}"
    cd $APK_DIR
}

