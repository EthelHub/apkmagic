function search_multi {
    DATE=`date +%x-%H:%M:%S`
    echo "[${DATE}] Search - Searching for interesting strings in $1... ($2.txt)"
    cd $1
    TEXTO=(user username password pass api key http)
    for PALABRA in ${TEXTO[*]}
    do
        find . -type f -print0 ! -path '*.svn*' | xargs -0 grep -i --exclude '*.min.*' --exclude '*.css' $PALABRA - >> $REPORT_DIR/$2
    done
    cd $DIR
}

function search_debug {
    DATE=`date +%x-%H:%M:%S`
    echo "[${DATE}] Search - Searching for debug initialization Strings.. (debug_words.txt)"
    cd $SOURCES_DIR
    find . -type f -print0 ! -path '*.svn*' | xargs -0 grep --exclude '*.min.*' --exclude '*.css' DEBUG | grep false | tr -s ' ' | cut -d':' -f 1 | sort >> $REPORT_DIR/debug_words.txt
}

