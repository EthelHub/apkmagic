#APK_DIR=`pwd`
#REPORT_DIR=${APK_DIR}/report

function cabecera_principal {
    echo "================================================================================"
    echo "$1"
    echo "================================================================================"
}

function cabecera {
    echo "" && echo "# $1" && echo ""
}

function report {
    ( 
        cabecera_principal  "REPORTE GENERAL"
        cat $REPORT_DIR/base_info.txt
        cabecera_principal "PERMISOS MANIFEST"
        cabecera "Permisos detectados :"
        cat $REPORT_DIR/permissions.txt
        echo ""
        cat $REPORT_DIR/dangerous.txt
        
        cabecera "FIN DEL REPORTE"
    ) > $APK_DIR/report.txt
}

#reporte